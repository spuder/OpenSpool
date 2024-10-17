# Rainbow Reel

RFID tracking system for 3d printer filament


Rainbow Reel is a combination of hardware and software that mimics the functionality of RFID tags on BambuLab filament roles. 

## Objectives

- Make it so easy to put RFID stickers on filament spools that your grandma could do it. 
- 100% programable using just a modern iphone/android (no RFID programmer or scripting).
- Change filament in 10 seconds using 1 hand. 
- 100% reliability so that filament can be changed with your eyes closed. 


![](/images/RainbowReel.png)




## Hardware

TODO: Document hardware requirements




### Hasn't this been done? 

Yes, people have tried setting up barcode scanners next to their printers. While that is doable, it is bespoke and clunky. Rainbow Reel attempts to duplicate the seamless expierence of BambuLab filaments with BambuLab Printers with AMS. 

### Can't the BambuLab RFID tags be reused? 

Yes, Sort of. People on ebay will sell their existing RFID tags. While you can tape the tag onto your spool, finding all the colors/types of RFID tags on filaments is time consuming and difficult

### Can't the RFID tags be cloned? 

Yes, sort of. If you have a proxmox3 or Flipper Zero and a lot of time, you can clone bambulabs tags. Its still in early development and isn't for the faint of heart. Checkout the awesome folks over at [Bambu-Research-Group/RFID-Tag-Guide](https://github.com/Bambu-Research-Group/RFID-Tag-Guide) on github. 