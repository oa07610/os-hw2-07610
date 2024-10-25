// Per-CPU state
struct cpu {
  uchar apicid;                // Local APIC ID
  struct context *scheduler;   // swtch() here to enter scheduler
  struct taskstate ts;         // Used by x86 to find stack for interrupt
  struct segdesc gdt[NSEGS];   // x86 global descriptor table
  volatile uint started;       // Has the CPU started?
  int ncli;                    // Depth of pushcli nesting.
  int intena;                  // Were interrupts enabled before pushcli?
  struct proc *proc;           // The process running on this cpu or null
};

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

extern struct cpu cpus[NCPU];
extern int ncpu;
int setnice(int pid, int nice_value);
void gettreeinfo(int *count, int *total_weight, int *period);
void getprocinfo(int pid, struct proc_info *info);
int treebalanced(void);

//PAGEBREAK: 17
// Saved registers for kernel context switches.
// Don't need to save all the segment registers (%cs, etc),
// because they are constant across kernel contexts.
// Don't need to save %eax, %ecx, %edx, because the
// x86 convention is that the caller has saved them.
// Contexts are stored at the bottom of the stack they
// describe; the stack pointer is the address of the context.
// The layout of the context matches the layout of the stack in swtch.S
// at the "Switch stacks" comment. Switch doesn't save eip explicitly,
// but it is on the stack and allocproc() manipulates it.
struct context {
  uint edi;
  uint esi;
  uint ebx;
  uint ebp;
  uint eip;
};

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

//This enumerator will be used to determine the color of each process in the red-black tree
enum procColor {RED, BLACK};	

// Per-process state
struct proc {
  uint sz;                     // Size of process memory (bytes)
  pde_t* pgdir;                // Page table
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  int pid;                     // Process ID
  struct proc *parent;         // Parent process
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // swtch() here to run process
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)
  
  // members for CFS
  double vruntime;    	// Time elapsed since the process was created
  int curr_runtime;		// Time process has run in the current scheduling round
  int time_slice;	// Maximum execution time of the process in the current scheduling round
  int nice_value;		// Used to determine the process's priority
  int weight;		// Used to determine the process's maximum execution time

  // members for red-black tree

  enum procColor color;
  struct proc *r;
  struct proc *l;
  struct proc *p;
};

// Process memory is laid out contiguously, low addresses first:
//   text
//   original data and bss
//   fixed-size stack
//   expandable heap
//Red-Black Tree data structure
struct rbtree {
  int length;
  int period;
  int total_weight;
  struct proc *root;
  struct proc *min_vruntime;
}; 

struct rbtree* gettree(void);