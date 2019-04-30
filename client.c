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
	printf("sfd == %d",sfd);
	
	struct sockaddr_in ser_addr;
	memset(&ser_addr,0,sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(2000);
	ser_addr.sin_addr.s_addr = inet_addr("113.55.58.199");
	

	if(connect(sfd,(struct sockaddr*)&ser_addr,sizeof(struct sockaddr)) == -1){
		printf("test");
		perror("connect");
		close(sfd);
		exit(-1);
	}
	
	char buf[512]={0};
	
	int epoll_fd = epoll_create(2);
	struct epoll_event event_,events[2];

	event_.data.fd = 0;
	event_.events = EPOLLIN;
	epoll_ctl(epoll_fd,EPOLL_CTL_ADD,0,&event_);

	event_.data.fd = sfd;
	event_.events = EPOLLIN;
	epoll_ctl(epoll_fd,EPOLL_CTL_ADD,sfd,&event_);
	
	while(1){
		memset(events,0,sizeof(events));
		epoll_wait(epoll_fd,events,2,0);
		for(int i = 0;i < 1; ++i){
			if(events[i].data.fd == sfd){
				recv(sfd,buf,512,0);
				printf("				service:|%s|\n",buf);
				memset(buf,0,512);
			}else if(events[i].data.fd == 0){
				memset(buf,0,512);
				scanf("%s",buf);
				send(sfd,buf,512,0);
				memset(buf,0,512);
			}
		}
	}
}
