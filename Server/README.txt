Run a server THEN clients.
Sometimes if the client doesn't connect to the server you may have to restart.

To kill the program:
Press esc and close the terminal window.
or alt+tab to the terminal and press ctrl+c.

Setting up the Config:
IMPORTANT
For Both:
players/numMinotaur should match on all clients and server
minotaurRadarOnly/musicProx/soundScaling should match everyone elses.
//IMPORTANT

For Clients:
Set the system config:
window-width/window-height/fullscreen/anti-alias desired resolution and settings.
sound/music/maxMusicVol/maxSoundVol to desired settings
minotaurRadarOnly/musicProx/soundScaling should match everyone elses.

For Server:
Physics, almost all player configs, gameRestart, serverTicks can be changed.





------
probably don't want to mess with (but are determined client side):
models
skybox

For modifying configurations it is suggested to use the config.txt and not modify masterConfig.txt.
If you delete the config.txt the server and client will read masterConfig.txt.

