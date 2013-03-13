CitySync
========

CitySync is a project to create a Multiplayer modifcation for SimCity 4 Deluxe/Rush hour

Attempts have been made in the past to provide this abilty but they all relied on off the shelf syncronisation software such as Dropbox or Sugar Sync. While these worked... there were always problems with the game writing to a neighbours city when playing, conflicts, a lack of permissions handling etc

This project is different as it is dedicated syncronisation software that understands (to some degree) the save game file format and will prevent mayors from modifying there neighbours cities without permission.


Phase 0
=======

Feasibility study

- Can we parse the save game format. Yes.  Done!
- What effect does ignoring changes to a neighbour have if we simply over write them is it game breaking?
- Are city GUIDs persistent over founding/obliterating a city
- If we lock a file so its completly read only will it fuck the game?
- Will two people be able to play side by side with out the game breaking ?
- How do we handle making network connections?


Phase 1
=======

Client/server with basic syncing and mutiplayer region view in launcher.
Launching the game, and monitoring save game changes/ pushing changes to server.
Support for mods/plugins.


Phase 2
=======
Networking mode


Future Features
=============
In game UI
Chat




