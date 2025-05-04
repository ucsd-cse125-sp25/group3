INIT_CONNECTION = 0,
Initial packet when starting the game; will include character type

KEY_EVENT = 3,
For client to communicate to server which key was pressed

STATE_UPDATE = 4 
For server to communicate to client which parts of the state was updated