# op-sys-p3
Implementing a memory allocator for the heap of a user-level process. Essentially implement a simple version of malloc() and free().  This involves managing a free list of memory and finding a contiguous chunk of memory that is large enough for the user's request; when the user later frees memory, it is added back to this list.
