/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "channels.h"
#include "helpers.h"

LimitsGroup::LimitsGroup(Firmware * firmware, TableLayout * tableLayout, int row, int col, int & value, const ModelData & model, int min, int max, int deflt, ModelPanel * panel):
  firmware(firmware),
  spinbox(new QDoubleSpinBox()),
  value(value),
  displayStep(0.1)
{
  Board::Type board = firmware->getBoard();
  bool allowGVars = IS_HORUS_OR_TARANIS(board);
  int internalStep = 1;

  spinbox->setProperty("index", row);
  spinbox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
  spinbox->setAccelerated(true);

  if (firmware->getCapability(PPMUnitMicroseconds)) {
    displayStep = 0.512;
    spinbox->setDecimals(1);
    spinbox->setSuffix("us");
  }
  else {
    spinbox->setDecimals(0);
    spinbox->setSuffix("%");
  }

  if (deflt == 0 /*it's the offset*/) {
    spinbox->setDecimals(1);
  }
  else {
    internalStep *= 10;
  }

  spinbox->setSingleStep(displayStep * internalStep);
  spinbox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

  QHBoxLayout *horizontalLayout = new QHBoxLayout();
  QCheckBox *gv = new QCheckBox(tr("GV"));
  gv->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  horizontalLayout->addWidget(gv);
  QComboBox *cb = new QComboBox();
  cb->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
  horizontalLayout->addWidget(cb);
  horizontalLayout->addWidget(spinbox);
  tableLayout->addLayout(row, col, horizontalLayout);
  gvarGroup = new GVarGroup(gv, spinbox, cb, value, model, deflt, min, max, displayStep, allowGVars);
  QObject::connect(gvarGroup, &GVarGroup::valueChanged, panel, &ModelPanel::modified);
}

LimitsGroup::~LimitsGroup()
{
  delete gvarGroup;
}

void LimitsGroup::setValue(int val)
{
  gvarGroup->setWeight(val);
}

void LimitsGroup::updateMinMax(int max)
{
  if (spinbox->maximum() == 0) {
    spinbox->setMinimum(-max * displayStep);
    gvarGroup->setMinimum(-max);
    if (value < -max) {
      value = -max;
    }
  }
  if (spinbox->minimum() == 0) {
    spinbox->setMaximum(max * displayStep);
    gvarGroup->setMaximum(max);
    if (value > max) {
      value = max;
    }
  }
}

Channels::Channels(QWidget * parent, ModelData & model, GeneralSettings & generalSettings, Firmware * firmware):
  ModelPanel(parent, model, generalSettings, firmware)
{
  Stopwatch s1("Channels");

  chnCapability = firmware->getCapability(Outputs);
  int channelNameMaxLen = firmware->getCapability(ChannelsName);

  QStringList headerLabels;
  headerLabels << "#";
  if (channelNameMaxLen > 0) {
    headerLabels << tr("Name");
  }
  headerLabels << tr("Subtrim") << tr("Min") << tr("Max") << tr("Direction");
  if (IS_HORUS_OR_TARANIS(firmware->getBoard()))
    headerLabels << tr("Curve");
  if (firmware->getCapability(PPMCenter))
    headerLabels << tr("PPM Center");
  if (firmware->getCapability(SYMLimits))
    headerLabels << tr("Linear Subtrim");
  TableLayout *tableLayout = new TableLayout(this, chnCapability, headerLabels);

  s1.report("header");

  for (int i = 0; i < chnCapability; i++) {
    int col = 0;

    // Channel label
    QLabel *label = new QLabel(this);
    label->setText(tr("CH%1").arg(i+1));
    label->setProperty("index", i);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    label->setContextMenuPolicy(Qt::CustomContextMenu);
    label->setToolTip(tr("Popup menu available"));
    label->setMouseTracking(true);
    connect(label, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomContextMenuRequested(QPoint)));
    tableLayout->addWidget(i, col++, label);

    // Channel name
    if (channelNameMaxLen > 0) {
      name[i] = new QLineEdit(this);
      name[i]->setProperty("index", i);
      name[i]->setMaxLength(channelNameMaxLen);
      QRegExp rx(CHAR_FOR_NAMES_REGEX);
      name[i]->setValidator(new QRegExpValidator(rx, this));
      connect(name[i], SIGNAL(editingFinished()), this, SLOT(nameEdited()));
      tableLayout->addWidget(i, col++, name[i]);
    }

    // Channel offset
    chnOffset[i] = new LimitsGroup(firmware, tableLayout, i, col++, model.limitData[i].offset, model, -1000, 1000, 0, this);

    // Channel min
    chnMin[i] = new LimitsGroup(firmware, tableLayout, i, col++, model.limitData[i].min, model, -model.getChannelsMax() * 10, 0, -1000, this);

    // Channel max
    chnMax[i] = new LimitsGroup(firmware, tableLayout, i, col++, model.limitData[i].max, model, 0, model.getChannelsMax() * 10, 1000, this);

    // Channel inversion
    invCB[i] = new QComboBox(this);
    invCB[i]->insertItems(0, QStringList() << tr("---") << tr("INV"));
    invCB[i]->setProperty("index", i);
    connect(invCB[i], SIGNAL(currentIndexChanged(int)), this, SLOT(invEdited()));
    tableLayout->addWidget(i, col++, invCB[i]);

    // Curve
    if (IS_HORUS_OR_TARANIS(firmware->getBoard())) {
      curveCB[i] = new QComboBox(this);
      curveCB[i]->setProperty("index", i);
      connect(curveCB[i], SIGNAL(currentIndexChanged(int)), this, SLOT(curveEdited()));
      tableLayout->addWidget(i, col++, curveCB[i]);
    }

    // PPM center
    int ppmCenterMax = firmware->getCapability(PPMCenter);
    if (ppmCenterMax) {
      centerSB[i] = new QSpinBox(this);
      centerSB[i]->setProperty("index", i);
      centerSB[i]->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
      centerSB[i]->setSuffix("us");
      centerSB[i]->setMinimum(1500 - ppmCenterMax);
      centerSB[i]->setMaximum(1500 + ppmCenterMax);
      centerSB[i]->setValue(1500);
      connect(centerSB[i], SIGNAL(editingFinished()), this, SLOT(ppmcenterEdited()));
      tableLayout->addWidget(i, col++, centerSB[i]);
    }

    // Symetrical limits
    if (firmware->getCapability(SYMLimits)) {
      symlimitsChk[i] = new QCheckBox(this);
      symlimitsChk[i]->setProperty("index", i);
      symlimitsChk[i]->setChecked(model.limitData[i].symetrical);
      connect(symlimitsChk[i], SIGNAL(toggled(bool)), this, SLOT(symlimitsEdited()));
      tableLayout->addWidget(i, col++, symlimitsChk[i]);
    }
  }
  update();
  s1.report("add elements");

  disableMouseScrolling();
  tableLayout->resizeColumnsToContents();
  tableLayout->pushRowsUp(chnCapability+1);
  s1.report("end");
}

Channels::~Channels()
{
  // compiler warning if delete[]
  for (int i = 0; i < CPN_MAX_CHNOUT; i++) {
    delete name[i];
    delete chnOffset[i];
    delete chnMin[i];
    delete chnMax[i];
    delete invCB[i];
    delete curveCB[i];
    delete centerSB[i];
    delete symlimitsChk[i];
  }
}

void Channels::symlimitsEdited()
{
  if (!lock) {
    QCheckBox *ckb = qobject_cast<QCheckBox*>(sender());
    int index = ckb->property("index").toInt();
    model->limitData[index].symetrical = (ckb->checkState() ? 1 : 0);
    emit modified();
  }
}

void Channels::nameEdited()
{
  if (!lock) {
    QLineEdit *le = qobject_cast<QLineEdit*>(sender());
    int index = le->property("index").toInt();
    strcpy(model->limitData[index].name, le->text().toLatin1());
    emit modified();
  }
}

void Channels::refreshExtendedLimits()
{
  int channelMax = model->getChannelsMax();

  for (int i = 0 ; i < CPN_MAX_CHNOUT; i++) {
    chnOffset[i]->updateMinMax(10 * channelMax);
    chnMin[i]->updateMinMax(10 * channelMax);
    chnMax[i]->updateMinMax(10 * channelMax);
  }
  emit modified();
}

void Channels::invEdited()
{
  if (!lock) {
    QComboBox *cb = qobject_cast<QComboBox*>(sender());
    int index = cb->property("index").toInt();
    model->limitData[index].revert = cb->currentIndex();
    emit modified();
  }
}

void Channels::curveEdited()
{
  if (!lock) {
    QComboBox *cb = qobject_cast<QComboBox*>(sender());
    int index = cb->property("index").toInt();
    model->limitData[index].curve = CurveReference(CurveReference::CURVE_REF_CUSTOM, cb->itemData(cb->currentIndex()).toInt());
    emit modified();
  }
}

void Channels::ppmcenterEdited()
{
  if (!lock) {
    QSpinBox *sb = qobject_cast<QSpinBox*>(sender());
    int index = sb->property("index").toInt();
    model->limitData[index].ppmCenter = sb->value() - 1500;
    emit modified();
  }
}

void Channels::update()
{
  for (int i = 0; i < chnCapability; i++) {
    updateLine(i);
  }
}

void Channels::updateLine(int i)
{
  lock = true;
  if (firmware->getCapability(ChannelsName) > 0) {
    name[i]->setText(model->limitData[i].name);
  }
  chnOffset[i]->setValue(model->limitData[i].offset);
  chnMin[i]->setValue(model->limitData[i].min);
  chnMax[i]->setValue(model->limitData[i].max);
  invCB[i]->setCurrentIndex((model->limitData[i].revert) ? 1 : 0);
  if (IS_HORUS_OR_TARANIS(firmware->getBoard())) {
    int numcurves = firmware->getCapability(NumCurves);
    curveCB[i]->clear();
    for (int j = -numcurves; j <= numcurves; j++) {
      curveCB[i]->addItem(CurveReference(CurveReference::CURVE_REF_CUSTOM, j).toString(model, false), j);
    }
    curveCB[i]->setCurrentIndex(model->limitData[i].curve.value + numcurves);
  }
  if (firmware->getCapability(PPMCenter)) {
    centerSB[i]->setValue(model->limitData[i].ppmCenter + 1500);
  }
  if (firmware->getCapability(SYMLimits)) {
    symlimitsChk[i]->setChecked(model->limitData[i].symetrical);
  }
  lock = false;
}

void Channels::cmPaste()
{
  QByteArray data;
  if (hasClipboardData(&data)) {
    memcpy(&model->limitData[selectedIndex], data.constData(), sizeof(LimitData));
    updateLine(selectedIndex);
    emit modified();
  }
}

void Channels::cmDelete()
{
  if (QMessageBox::question(this, CPN_STR_APP_NAME, tr("Delete Channel. Are you sure?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    return;

  memmove(&model->limitData[selectedIndex], &model->limitData[selectedIndex + 1], (CPN_MAX_CHNOUT - (selectedIndex + 1)) * sizeof(LimitData));
  model->limitData[chnCapability - 1].clear();
  model->updateAllReferences(ModelData::REF_UPD_TYPE_CHANNEL, ModelData::REF_UPD_ACT_SHIFT, selectedIndex, 0, -1);

  for (int i = selectedIndex; i < chnCapability; i++) {
    updateLine(i);
  }

  emit modified();
}

void Channels::cmCopy()
{
  QByteArray data;
  data.append((char*)&model->limitData[selectedIndex], sizeof(LimitData));
  QMimeData *mimeData = new QMimeData;
  mimeData->setData(MIMETYPE_CHANNEL, data);
  QApplication::clipboard()->setMimeData(mimeData,QClipboard::Clipboard);
}

void Channels::cmCut()
{
  if (QMessageBox::question(this, CPN_STR_APP_NAME, tr("Cut Channel. Are you sure?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    return;

  cmCopy();
  cmClear(false);
}

void Channels::onCustomContextMenuRequested(QPoint pos)
{
  QLabel *label = (QLabel *)sender();
  selectedIndex = label->property("index").toInt();
  QPoint globalPos = label->mapToGlobal(pos);

  QMenu contextMenu;
  contextMenu.addAction(CompanionIcon("copy.png"), tr("Copy"),this,SLOT(cmCopy()));
  contextMenu.addAction(CompanionIcon("cut.png"), tr("Cut"),this,SLOT(cmCut()));
  contextMenu.addAction(CompanionIcon("paste.png"), tr("Paste"),this,SLOT(cmPaste()))->setEnabled(hasClipboardData());
  contextMenu.addAction(CompanionIcon("clear.png"), tr("Clear"),this,SLOT(cmClear()));
  contextMenu.addSeparator();
  contextMenu.addAction(CompanionIcon("arrow-right.png"), tr("Insert"),this,SLOT(cmInsert()))->setEnabled(insertAllowed());
  contextMenu.addAction(CompanionIcon("arrow-left.png"), tr("Delete"),this,SLOT(cmDelete()));
  contextMenu.addAction(CompanionIcon("moveup.png"), tr("Move Up"),this,SLOT(cmMoveUp()))->setEnabled(moveUpAllowed());
  contextMenu.addAction(CompanionIcon("movedown.png"), tr("Move Down"),this,SLOT(cmMoveDown()))->setEnabled(moveDownAllowed());
  contextMenu.addSeparator();
  contextMenu.addAction(CompanionIcon("clear.png"), tr("Clear All"),this,SLOT(cmClearAll()));

  contextMenu.exec(globalPos);
}

bool Channels::hasClipboardData(QByteArray * data) const
{
  const QClipboard * clipboard = QApplication::clipboard();
  const QMimeData * mimeData = clipboard->mimeData();
  if (mimeData->hasFormat(MIMETYPE_CHANNEL)) {
    if (data)
      data->append(mimeData->data(MIMETYPE_CHANNEL));
    return true;
  }
  return false;
}

bool Channels::insertAllowed() const
{
  return ((selectedIndex < chnCapability - 1) && (model->limitData[chnCapability - 1].isEmpty()));
}

bool Channels::moveDownAllowed() const
{
  return selectedIndex < chnCapability - 1;
}

bool Channels::moveUpAllowed() const
{
  return selectedIndex > 0;
}

void Channels::cmMoveUp()
{
  swapData(selectedIndex, selectedIndex - 1);
}

void Channels::cmMoveDown()
{
  swapData(selectedIndex, selectedIndex + 1);
}

void Channels::cmClear(bool prompt)
{
  if (prompt) {
    if (QMessageBox::question(this, CPN_STR_APP_NAME, tr("Clear Channel. Are you sure?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
      return;
  }

  model->limitData[selectedIndex].clear();
  model->updateAllReferences(ModelData::REF_UPD_TYPE_CHANNEL, ModelData::REF_UPD_ACT_CLEAR, selectedIndex);
  updateLine(selectedIndex);
  emit modified();
}

void Channels::cmClearAll()
{
  if (QMessageBox::question(this, CPN_STR_APP_NAME, tr("Clear all Channels. Are you sure?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    return;

  for (int i = 0; i < chnCapability; i++) {
    model->limitData[i].clear();
    model->updateAllReferences(ModelData::REF_UPD_TYPE_CHANNEL, ModelData::REF_UPD_ACT_CLEAR, i);
    updateLine(i);
  }
  emit modified();
}

void Channels::cmInsert()
{
  memmove(&model->limitData[selectedIndex + 1], &model->limitData[selectedIndex], (CPN_MAX_CHNOUT - (selectedIndex + 1)) * sizeof(LimitData));
  model->limitData[selectedIndex].clear();
  model->updateAllReferences(ModelData::REF_UPD_TYPE_CHANNEL, ModelData::REF_UPD_ACT_SHIFT, selectedIndex, 0, 1);
  update();
  emit modified();
}

void Channels::swapData(int idx1, int idx2)
{
  if ((idx1 != idx2) && (!model->limitData[idx1].isEmpty() || !model->limitData[idx2].isEmpty())) {
    LimitData chntmp = model->limitData[idx2];
    LimitData *chn1 = &model->limitData[idx1];
    LimitData *chn2 = &model->limitData[idx2];
    memcpy(chn2, chn1, sizeof(LimitData));
    memcpy(chn1, &chntmp, sizeof(LimitData));
    model->updateAllReferences(ModelData::REF_UPD_TYPE_CHANNEL, ModelData::REF_UPD_ACT_SWAP, idx1, idx2);
    updateLine(idx1);
    updateLine(idx2);
    emit modified();
  }
}
