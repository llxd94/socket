#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
int main(int argc,char *argv[]){
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if (sfd == -1){
		perror("socket");
		exit(-1);
	}
	
	struct sockaddr_in my_addr;
	memset(&my_addr,0,sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(2000);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	

	if(bind(sfd,(const struct sockaddr *)&my_addr,sizeof(my_addr)) == -1){
		perror("bind");
		exit(-1);
	};

	if(listen(sfd,2) == -1){
		perror("listen");
		exit(-1);
	};

	struct sockaddr_in client_addr;
	memset(&client_addr,0,sizeof(my_addr));
	int addrlen = sizeof(struct sockaddr);
	int new_fd = accept(sfd,(struct sockaddr *)&client_addr,(socklen_t * )&addrlen);
	if (new_fd == -1){
		perror("accept");
		exit(-1);
	}
	printf("aclient link in: client_addr  %u, cliend _port == %d\n",client_addr.sin_addr.s_addr,client_addr.sin_port);
	
	char buf[512] = {0};
	send(new_fd,"success in connect we can chat",512,0);


	struct epoll_event event_,events[2];

	int epoll_fd = epoll_create(2);

	event_.data.fd = new_fd;
	event_.events = EPOLLIN ;
	epoll_ctl(epoll_fd,EPOLL_CTL_ADD,new_fd,&event_);  

	event_.data.fd = 0;
	event_.events = EPOLLIN  ;
	epoll_ctl(epoll_fd,EPOLL_CTL_ADD,0,&event_);
	
	
	while(1){
		memset(events,0,sizeof(events));
		epoll_wait(epoll_fd,events,2,0);
		for(int i=0;i<2;++i){
			if(events[i].data.fd== 0){
				scanf("%s",buf);
				send(new_fd,buf,512,0);
				memset(buf,0,512);
				break;
			}
			if (events[i].data.fd == new_fd){
				recv(new_fd,buf,512,0);
				printf("				client:|%s|\n",buf);
				memset(buf,0,512);
				break;
			}
		}
	}

}
