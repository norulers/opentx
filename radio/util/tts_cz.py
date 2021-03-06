# -*- coding: utf-8 -*-

# Czech language sounds configuration

from tts_common import filename


systemSounds = []
sounds = []

for i, s in enumerate(["nula", "jedna", "dva"]):
    systemSounds.append((s, filename(i)))
for i in range(97):
    systemSounds.append((str(3 + i), filename(3 + i)))
for i, s in enumerate(["sto", "dvěsta", "třista", "čtyřista", "pětset", "šestset", "sedmset", "osmset", "devětset", "tisíc"]):
    systemSounds.append((s, filename(100 + i)))
for i, s in enumerate(["tisíce", "jeden", "jedno", "dvě", "celá", "celé", "celých", "mínus"]):
    systemSounds.append((s, filename(110 + i)))
for i, (s, f) in enumerate([("volt", "volt0"), ("volty", "volt1"), ("voltů", "volt2"),("voltu", "volt3"),
                            ("ampér", "amp0"), ("ampéry", "amp1"), ("ampérů", "amp2"), ("ampéru", "amp3"),
                            ("miliampér", "mamp0"), ("miliampéry", "mamp1"), ("miliampérů", "mamp2"), ("miliampéru", "mamp3"),
                            ("uzel", "knot0"), ("uzly", "knot1"), ("uzlů", "knot2"), ("uzlu", "knot3"),
                            ("metr za sekundu", "mps0"), ("metry za sekundu", "mps1"), ("metrů za sekundu", "mps2"), ("metru za sekundu", "mps3"),
                            ("stopa za sekundu", "fps0"), ("stopy za sekundu", "fps1"), ("stop za sekundu", "fps2"), ("stopy za sekundu", "fps3"),
                            ("kilometr za hodinu", "kph0"), ("kilometry za hodinu", "kph1"), ("kilometrů za hodinu", "kph2"), ("kilometru za hodinu", "kph3"),
                            ("míle za hodinu", "mph0"), ("míle za hodinu", "mph1"), ("mil za hodinu", "mph2"), ("míle za hodinu", "mph3"),
                            ("metr", "meter0"), ("metry", "meter1"), ("metrů", "meter2"), ("metru", "meter3"),
                            ("stopa", "foot0"), ("stopy", "foot1"), ("stop", "foot2"),("stopy", "foot3"),
                            ("stupeň celsia", "celsius0"), ("stupně celsia", "celsius1"), ("stupňů celsia", "celsius2"), ("stupně celsia", "celsius3"),
                            ("stupeň fahrenheita", "fahr0"), ("stupně fahrenheita", "fahr1"), ("stupňů fahrenheita", "fahr2"), ("stupně fahrenheita", "fahr3"),
                            ("procento", "percent0"), ("procenta", "percent1"), ("procent", "percent2"), ("procenta", "percent3"),
                            ("miliampérhodina", "mamph0"), ("miliampérhodiny", "mamph1"), ("miliampérhodin", "mamph2"), ("miliampérhodiny", "mamph3"),
                            ("vat", "watt0"), ("vaty", "watt1"), ("vatů", "watt2"), ("vatu", "watt3"),
                            ("milivat", "mwatt0"), ("milivaty", "mwatt1"), ("milivatů", "mwatt2"), ("milivatu", "mwatt3"),
                            ("decibel", "db0"), ("decibely", "db1"), ("decibelů", "db2"), ("decibelu", "db3"),
                            ("otáčka za minutu", "rpm0"), ("otáčky za minutu", "rpm1"), ("otáček za minutu", "rpm2"), ("otáčky za minutu", "rpm3"),
                            ("gé", "g0"), ("gé", "g1"), ("gé", "g2"), ("gé", "g3"),
                            ("stupeň", "degree0"), ("stupně", "degree1"), ("stupňů", "degree2"), ("stupně", "degree3"),
                            ("radián", "rad0"), ("radiány", "rad1"), ("radiánů", "rad2"), ("radiánu", "rad3"),
                            ("mililitr", "ml0"), ("mililitry", "ml1"), ("mililitrů", "ml2"), ("mililitru", "ml3"),
                            ("dutá unce", "founce0"), ("duté unce", "founce1"), ("dutých uncí", "founce2"), ("duté unce", "founce3"),
                            ("milliliter per minute", "mlpm0"), ("milliliters per minute", "mlpm1"),
                            ("hodina", "hour0"), ("hodiny", "hour1"), ("hodin", "hour2"), ("hodiny", "hour3"),
                            ("minuta", "minute0"), ("minuty", "minute1"), ("minut", "minute2"),  ("minuty", "minute3"),
                            ("sekunda", "second0"), ("sekundy", "second1"), ("sekund", "second2"), ("sekundy", "second3"),
                            ]):
    systemSounds.append((s, filename(f)))
for s, f in [("střed trimu", "midtrim"),
             ("maximum trimu", "maxtrim"),
             ("minimum trimu", "mintrim"),
             ("stopky 1 doběhly", "timovr1"),
             ("stopky 2 doběhly", "timovr2"),
             ("stopky 3 doběhly", "timovr3"),
             ("baterie rádia je vybitá", "lowbatt"),
             ("zapoměl jsi na mě!", "inactiv"),
             ("plyn není na nule", "thralert"),
             ("přepínače nejsou ve výchozí poloze", "swalert"),
             ("neplatná eeprom", "eebad"),
             ("Žádám o povolení ke startu.", "hello"),
             ("Tak zase příště!", "bye"),
             ("dvacet sekund", "timer20"),
             ("třicet sekund", "timer30"),
             ("A1,nízká", "a1_org"),
             ("A1,kritická", "a1_red"),
             ("A2,nízká", "a2_org"),
             ("A2,kritická", "a2_red"),
             ("A3,nízká", "a3_org"),
             ("A3,kritická", "a3_red"),
             ("A4,nízká", "a4_org"),
             ("A4,kritická", "a4_red"),
             ("nízký RF signál", "rssi_org"),
             ("kritický RF signál", "rssi_red"),
             ("problém s anténou rádia", "swr_red"),
             ("telemetrie ztracena", "telemko"),
             ("telemetrie je funkční", "telemok"),
             ("trenér port odpojen", "trainko"),
             ("trenér port připojen", "trainok"),
             ("senzor ztracen", "sensorko"),
             ("servo přetíženo", "servoko"),
             ("r f přebuzeno", "rxko"),
             ("receiver still connected", "modelpwr"),
             ]:
    systemSounds.append((s, filename(f)))
for i, (s, f) in enumerate([("podvozek je zasunut", "podvo0"),
                            ("podvozek je vysunut", "podvo1"),
                            ("klapky jsou zapnuty", "klapk1"),
                            ("klapky jsou vypnuty", "klapk0"),
                            ("trenér je zapnut", "trene1"),
                            ("trenér je vypnutý", "trene0"),
                            ("motor je vypnut", "motor0"),
                            ("motor je aktivován", "motor1"),
                            ("jsi příliš vysoko", "vysoko"),
                            ("jsi příliš nízko", "nizko"),
                            ("baterie modelu je vybitá", "bat0"),
                            ("dochází palivo", "palivo"),
                            ("nízký signál!", "nsigna"),
                            ("kritický signál!", "ksigna"),
                            ("rychlý režim aktivován", "rrychl"),
                            ("termický režim aktivován", "rtermi"),
                            ("normální režim aktivován", "rnorm"),
                            ("režim start aktivován", "rstart"),
                            ("režim přistání aktivován", "prista"),
                            ("vario je zapnuto", "vario0"),
                            ("vario je vypnuto", "vario1"),
                            ("start povolen", "start1"),
                            ("letový režim jedna", "fm-1"),
                            ("letový režim dva", "fm-2"),
                            ("letový režim tři", "fm-3"),
                            ("letový režim čtyři", "fm-4"),
                            ("letový režim pět", "fm-5"),
                            ("letový režim šest", "fm-6"),
                            ("letový režim sedm", "fm-7"),
                            ("letový režim osm", "fm-8"),
                            ]):
    sounds.append((s, filename(f)))
