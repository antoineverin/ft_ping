#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

uint16_t checksum(void *data, int len) {
  uint16_t *buf = data;
  uint32_t sum = 0;

  for (int i = 0; i < len; i += 2)
    sum += *buf++;

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  return ~sum;
}

int main(int argc, char  *argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "v?")) != -1) {
    switch (opt) {
      case 'v':
        exit(EXIT_SUCCESS);
        break;
      default:
        exit(EXIT_FAILURE);
    }
  }

  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (sockfd < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  struct icmphdr hdr;
  hdr.type = ICMP_ECHO;
  hdr.code = 0;
  hdr.checksum = 0;
  hdr.un.echo.id = getpid();
  hdr.un.echo.sequence = 1;
  hdr.checksum = checksum(&hdr, sizeof(hdr));

  struct sockaddr_in dest;
  dest.sin_family = AF_INET;
  inet_pton(AF_INET, "8.8.8.8", &dest.sin_addr);

  sendto(sockfd, &hdr, sizeof(hdr), 0, (struct sockaddr*) &dest, sizeof(dest));

  char buf[1024] = {0};
  socklen_t addr_len = sizeof(dest);
  recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*) &dest, &addr_len);

  struct iphdr *iphdr = (struct iphdr*) buf;
  struct icmphdr *res = (struct icmphdr*) (buf + iphdr->ihl * 4);

  printf("reply:\n type: %x\n code: %x\n id: %x\n seq: %x\n checksum: %x\n", res->type, res->code, res->un.echo.id, res->un.echo.sequence, res->checksum);

  return EXIT_SUCCESS;
}
