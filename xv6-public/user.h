struct stat;
struct rtcdate;
struct proc_info {
  int pid;
  int nice_value;
  int weight;
  double vruntime;
  int curr_runtime;
};
struct rb_node_info {
  int pid;
  double vruntime;
  int color;      // 0 for RED, 1 for BLACK
  int left_pid;
  int right_pid;
  int parent_pid;
};

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int gettreeinfo(int *count, int *total_weight, int *period);
int getprocinfo(int pid, struct proc_info *info);
int gettreenodes(int max_nodes, struct rb_node_info *nodes);
int treebalanced(void);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
