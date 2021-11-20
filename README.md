# Comunication-System-EX2-UDP-client-Server
this is a University Home exersice - UDP client and server

The entire process can be broken down into the following steps : 

UDP Server :  

1.Create a UDP socket.
2.Bind the socket to the server address.
3.Wait until the datagram packet arrives from the client.
4.Process the datagram packet and send a reply to the client.
5.Go back to Step 3.


UDP Client :  

1.Create a UDP socket.
2.Send a message to the server.
3.Wait until response from the server is received.
4.Process reply and go back to step 2, if necessary.
5.Close socket descriptor and exit.