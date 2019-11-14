#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct rtnl_handle {
    int         fd;
    struct sockaddr_nl  local;
    struct sockaddr_nl  peer;
    __u32           dump;
    int         proto;
    FILE               *dump_fp;
#define RTNL_HANDLE_F_LISTEN_ALL_NSID       0x01
    int         flags;
};

#define NLM_F_REQUEST       1   /* It is request message.   */
#define NLM_F_MULTI     2   /* Multipart message, terminated by NLMSG_DONE */
#define NLM_F_ACK       4   /* Reply with ack, with zero or error code */
#define NLM_F_ECHO      8   /* Echo this request        */
#define NLM_F_DUMP_INTR     16  /* Dump was inconsistent due to sequence change */
#define NLM_F_DUMP_FILTERED 32  /* Dump was filtered as requested */

/* Modifiers to GET request */
#define NLM_F_ROOT  0x100   /* specify tree root    */
#define NLM_F_MATCH 0x200   /* return all matching  */
#define NLM_F_ATOMIC    0x400   /* atomic GET       */
#define NLM_F_DUMP  (NLM_F_ROOT|NLM_F_MATCH)


int main() {

    int proto = 0;
    int fd;
    struct sockaddr_nl local;
    struct sockaddr_nl peer;
    __u32 seq;
    __u32 dump;

    /*  open socket   */
    socklen_t addr_len;
    int sndbuf = 32768;
    int rcvbuf = 1024 * 1024;

    memset(rth, 0, sizeof(*rth));

    fd = socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
    if (fd < 0) {
        perror("Cannot open netlink socket");
        return -1;
    }

    if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF,
               &sndbuf, sizeof(sndbuf)) < 0) {
        perror("SO_SNDBUF");
        return -1;
    }

    if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF,
               &rcvbuf, sizeof(rcvbuf)) < 0) {
        perror("SO_RCVBUF");
        return -1;
    }

    memset(&local, 0, sizeof(local));
    local.nl_family = AF_NETLINK;
    local.nl_groups = 0;

    if (bind(fd, (struct sockaddr *)&local,
         sizeof(local)) < 0) {
        perror("Cannot bind netlink socket");
        return -1;
    }
    addr_len = sizeof(local);
    if (getsockname(fd, (struct sockaddr *)&local,
            &addr_len) < 0) {
        perror("Cannot getsockname");
        return -1;
    }
    if (addr_len != sizeof(local)) {
        fprintf(stderr, "Wrong address length %d\n", addr_len);
        return -1;
    }
    if (local.nl_family != AF_NETLINK) {
        fprintf(stderr, "Wrong address family %d\n",
            local.nl_family);
        return -1;
    }
    seq = time(NULL);
    /*  open socket end  */

    /*  send msg  */
    struct {
        struct nlmsghdr nlh;
        struct ifinfomsg ifm;
        char buf[1024];
    } req = {
        .nlh.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg)),
        .nlh.nlmsg_type = RTM_GETLINK,
        .nlh.nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST,
        .nlh.nlmsg_seq = dump = ++seq,
        .ifm.ifi_family = AF_UNSPEC,
    };
    /*  send msg end */
}
