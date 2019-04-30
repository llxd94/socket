client service:client.c service.c
	gcc client.c -o client 
	gcc service.c -o service 

clean:
	rm -rf service
	rm -rf client
