Winsock UDP Chat by Theo Kandiliotis
		 ionikh@hol.gr
		 http://www.geocities.com/SiliconValley/
		 Network/5045

This is a Visual Basic 6 application that uses the powerful
Winsock control to send and recieve data amongst two remote
computers.It's the simplest possible chat system for 1 on 1
realtime chat. Before you can chat with your partner,you both
need to fill in 3 fields. 

Connect With IP:
	In here,you type the other guy's IP adress (xxx.xxx.xxx.xxx)
Local Port:
	This can be any integer,say 1000. If you set your local port
	to 1000,your chat partner has to set his remote port at
	1000 as well. Ofcourse you have to do the same with your 
	remote port,it has to match his local port.
Remote Port:
	The integer that your chat partner uses as his local port.

e.g. if you use these settings:

Connect With IP: hisip
Local Port: 1000
Remote Port: 1001

the other guy should have these:

Connect With IP: yourip
Local Port: 1001
Remote Port: 1000


After you make these settings,click connect and you're ready
to chat with the remote computer. Your text will be sent
across the socket one sentense at a time,when you press enter. 

Have fun chatting !
