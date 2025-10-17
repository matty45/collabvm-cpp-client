[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/matty45/collabvm-qt-client)

This is a work in progress client for CollabVM made in C++, this might not go anywhere. Project is just an excuse for me to get coding again and also to learn Qt.

Plan: Create a full-featured chat-only client, then once that is done, begin to implement the stuff needed to control and view vms.

## Todo:
- [x] Implement VM List UI
- [ ] Multi-client support.
- [ ] Add error handling and validation for as much as possible.
- [ ] Properly deallocate client instances.
- [X] Dynamically update the VM list by gathering relevant information from websocket servers.
- [ ] Keep track of what vm belongs to which server.
- [ ] Allow for connecting to all servers simultaneously without connecting to any specific vm. (allows users to have notifications if a specific user joins a specific vm on a server, etc)
- [ ] Probe system for listing all the vms without making a persistent connection to them. 
- [ ] Implement VM chat UI.
- [ ] Implement configuration settings. (What servers to connect to, themes, etc.) 
