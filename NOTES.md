#PN532

https://forum.dangerousthings.com/t/success-pn532-is-not-easy-to-work-with/1108

https://stackoverflow.com/questions/43563378/how-do-i-read-passive-nfc-rfid-units-with-pn532

Beware some PN532 are clones and don't work as well

https://forum.dangerousthings.com/t/success-pn532-is-not-easy-to-work-with/1108/10


# Filament Mappings
WolfWithSword identified these mappings
```
// "func": "// Bambu spools are not needed for translation if they have the rfid tags
// but this can be used to overwrite them for HA displaying only
// This only overwrites the tray_type and tray_sub_brands, if you want other fields overwritten you can add them
// e.g.  tray_diameter, tray_weight, temps etc

var PolyLite_PLA = {
    \"tray_info_idx\": \"GFL00\", 
    \"tray_type\": \"PLA\",
    \"tray_sub_brands\": \"PolyLite PLA\"
}

var PolyTerra_PLA = {
    \"tray_info_idx\": \"GFL01\",
    \"tray_type\": \"PLA\",
    \"tray_sub_brands\": \"PolyTerra PLA\"
}

var Bambu_ABS = {
    \"tray_info_idx\": \"GFB00\",
    \"tray_type\": \"ABS\"
}

var Bambu_PACF = {
    \"tray_info_idx\": \"GFN03\",
    \"tray_type\": \"PA-CF\"
}

var Bambu_PC = {
    \"tray_info_idx\": \"GFC00\",
    \"tray_type\": \"PC\"
}

var Bambu_PLA_Basic = {
    \"tray_info_idx\": \"GFA00\",
    \"tray_type\": \"PLA\",
    \"tray_sub_brands\": \"PLA Basic\"
}
var Bambu_PLA_Matte = {
    \"tray_info_idx\": \"GFA01\",
    \"tray_type\": \"PLA\",
    \"tray_sub_brands\": \"PLA Matte\"
}

var Support_G = {
    \"tray_info_idx\": \"GFS01\",
    \"tray_type\": \"Support\",
    \"tray_sub_brands\": \"Support G\"
}

var Support_W = {
    \"tray_info_idx\": \"GFS00\",
    \"tray_type\": \"Support\",
    \"tray_sub_brands\": \"Support W\"
}

var Bambu_TPU_95A = {
    \"tray_info_idx\": \"GFU01\",
    \"tray_type\": \"TPU\",
    \"tray_sub_brands\": \"TPU 95A\"
}

var Generic_ABS = {
    \"tray_info_idx\": \"GFB99\",
    \"tray_type\": \"ABS\",
    \"tray_sub_brands\": \"ABS\"
}

var Generic_ASA = {
    \"tray_info_idx\": \"GFB98\",
    \"tray_type\": \"ASA\",
    \"tray_sub_brands\": \"ASA\"
}

var Generic_PA = {
    \"tray_info_idx\": \"GFN99\",
    \"tray_type\": \"PA\",
    \"tray_sub_brands\": \"PA\"
}

var Generic_PACF = {
    \"tray_info_idx\": \"GFN98\",
    \"tray_type\": \"PA-CF\",
    \"tray_sub_brands\": \"PA-CF\"
}

var Generic_PC = {
    \"tray_info_idx\": \"GFC99\",
    \"tray_type\": \"PC\",
    \"tray_sub_brands\": \"PC\"
} 

var Generic_PETG = {
    \"tray_info_idx\": \"GFG99\",
    \"tray_type\": \"PETG\",
    \"tray_sub_brands\": \"PETG\"
}
var Generic_PLA = {
    \"tray_info_idx\": \"GFL99\",
    \"tray_type\": \"PLA\",
    \"tray_sub_brands\": \"PLA\"
}
var Generic_PLACF = {
    \"tray_info_idx\": \"GFL98\",
    \"tray_type\": \"PLA-CF\",
    \"tray_sub_brands\": \"PLA-CF\"
}
var Generic_PVA = {
    \"tray_info_idx\": \"GFS99\",
    \"tray_type\": \"PVA\",
    \"tray_sub_brands\": \"PVA\"
}
var Generic_TPU = {
    \"tray_info_idx\": \"GFU99\",
    \"tray_type\": \"TPU\",
    \"tray_sub_brands\": \"TPU\"
}

var Bambu_PETCF = {
    \"tray_info_idx\": \"GFT00\",
    \"tray_type\": \"PET-CF\"
}

var Bambu_PLA_Impact = {
    \"tray_info_idx\": \"GFA03\",
    \"tray_type\": \"PLA\",
    \"tray_sub_brands\": \"PLA Impact\"
}

var Bambu_PLA_Metal = {
    \"tray_info_idx\": \"GFA02\",
    \"tray_type\": \"PLA\",
    \"tray_sub_brands\": \"PLA Metal\"
}

var filament_library = {
    \"GFU99\": Generic_TPU,
    \"GFS99\": Generic_PVA,
    \"GFL98\": Generic_PLACF,
    \"GFL99\": Generic_PLA,
    \"GFG99\": Generic_PETG,
    \"GFC99\": Generic_PC,
    \"GFN98\": Generic_PACF,
    \"GFN99\": Generic_PA,
    \"GFB98\": Generic_ASA,
    \"GFB99\": Generic_ABS,
    \"GFU01\": Bambu_TPU_95A,
    \"GFS00\": Support_W,
    \"GFS01\": Support_G,
    \"GFA01\": Bambu_PLA_Matte,
    \"GFA00\": Bambu_PLA_Basic,
    \"GFC00\": Bambu_PC,
    \"GFN03\": Bambu_PACF,
    \"GFB00\": Bambu_ABS,
    \"GFL01\": PolyTerra_PLA,
    \"GFL00\": PolyLite_PLA
}

if (msg.payload.print.ams !== undefined) {
    for (var ams of msg.payload.print.ams.ams) {
        for (var tray of ams.tray) {
            if (tray.tray_info_idx !== undefined && tray.tray_info_idx !== \"\") {
                if(filament_library.hasOwnProperty(tray.tray_info_idx)) {
                    var match = filament_library[tray.tray_info_idx];
                    tray.tray_type = match.tray_type;
                    tray.tray_sub_brands = match.tray_sub_brands;
                    if (tray.tray_diameter == \"0.00\") {
                        tray.tray_diameter = \"1.75\";
                    }
                }
            }
        }
    }
}
if(msg.payload.print.vt_tray != undefined) {
    if (msg.payload.print.vt_tray.tray_info_idx != undefined) {
        if (filament_library.hasOwnProperty(msg.payload.print.vt_tray.tray_info_idx)) {
            var match = filament_library[msg.payload.print.vt_tray.tray_info_idx];
            msg.payload.print.vt_tray.tray_type = match.tray_type;
            msg.payload.print.vt_tray.tray_sub_brands = match.tray_sub_brands;
            if (msg.payload.print.vt_tray.tray_diameter == \"0.00\") {
                msg.payload.print.vt_tray.tray_diameter = \"1.75\";
            }
        }
    }
}

node.send(msg);",
```