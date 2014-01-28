# Introduction
A douban fm client, with remote control.

# Remote control
Remote control is done with zmq. There're 2 sockets in the daemon.

* sock_rep, a REP socket to reply client request.
* sock_pub, a PUB socket to publish any status change to client.

## REP socket
For skip, rate, trash, switch, play & pause command, reply immediately after command is received, further process will continue and publish via sock_pub.

### info
Get current track info.

req: ["info"]
rep: ["error", "message"] or ["ok", track_info]

### skip
Skip current track.

req: ["skip"]
rep: ["error", "message"] or ["ok"]
### rate
Rate or unrate current track.

req: ["rate"]
rep: ["error", "message"] or ["ok"]

### trash
Trash current track

req: ["trash"]
rep: ["error", "message"] or ["ok"]

### switch
Switch channel

req: ["switch", channel_id]
rep: ["error", "message"] or ["ok"]

### play
Play track

req: ["play"]
rep: ["ok"]

### pause
Pause track

req: ["pause"]
req: ["ok"]

## PUB socket
Publish any player status change to client.

### info
msg: ["info", track_info(json string) ]

### pos
msg: ["pos", position(int) ]

### rate
msg: ["rate", like(bool) ]

### status
msg: ["status", "playing"/"paused"/etc.]
