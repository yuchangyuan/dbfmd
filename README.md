# Introduction
A douban fm client, with remote control.

# Installation

For Ubuntu 14.04, please install follow dependency:
- libzmq3-dev
- qtmultimedia5-dev
- libqt5multimedia5-plugins
- gstreamer0.10-fluendo-mp3

# Usage
## Login
Run ``dbfmd`` without any parameter will do login operation. The program will prompt for email and password, then output the server response in json format.

Typical usage will be first write a plain text file, say "secret.txt", with first line is email of the account and second line is the password of the account, then run below command:
```
cat secret.txt | ./dbfmd > login.json
```
The server response will redirect to file "login.json", this file can be used later.

## Play
Run ``dbfmd`` with exact one argument will cause the application connect to douban fm server and play the music. The first argument will be the path of "login.json" file which get from previous login step, or any non-exsistence file for play the music without login.

The daemon will listen to port 12677 & 12678 port for remote control. Currently, these two port are not configurable, it's necessary to change the code to use alternative port. There is a CLI application [dbfmc](https://github.com/yuchangyuan/dbfmc) demonstrate the usage of remote contorl. For detail of the protocol, see "Remote control" section below.

# Remote control
Remote control is done with zmq. There're 2 sockets in the daemon.

* sock_rep, a REP socket to reply any client request.
* sock_pub, a PUB socket to publish any status change to the client.

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

req: ["rate", like]
rep: ["error", "message"] or ["ok"]

Argument 'like' is optional, rate when "true", unrate otherwise, toggle when not exist.

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
msg: ["info", track_info: json ]

### pos
msg: ["pos", position: int ]

### rate
msg: ["rate", like: bool ]

### status
msg: ["status", "playing"/"paused"]

# Reference
* [豆瓣FM API](https://github.com/zonyitoo/doubanfm-qt/wiki/%E8%B1%86%E7%93%A3FM-API)
* [douban-fm-api](https://github.com/akfish/fm-terminal/blob/master/douban-fm-api.md)
* [douban.fm](https://github.com/turingou/douban.fm)
