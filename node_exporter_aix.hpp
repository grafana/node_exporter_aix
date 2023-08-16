#include <vector>
#include <string>

#define PART_COMPAT       (1<<0)
#define PART_CPU          (1<<1)
#define PART_DISKADAPTER  (1<<2)
#define PART_DISKPATH     (1<<3)
#define PART_MEM_PAGES    (1<<4)
#define PART_MEM          (1<<5)
#define PART_DISK         (1<<6)
#define PART_NETINTERFACE (1<<7)
#define PART_NETADAPTER   (1<<8)
#define PART_NETBUFFER    (1<<9)
#define PART_PARTITION    (1<<10)
#define PART_FILESYSTEMS  (1<<11)
#define PART_VMSTAT_V     (1<<11)
#define PART_FCSTAT_E     (1<<12)
#define PART_CPU_UTILS      (1<<13)
#define PART_PROCESS (1<<14)

extern int start_server(int port, int flags);

extern std::string generate_static_labels();

extern void gather_cpus_compat(std::ostream& response, const std::string& static_labels);
extern void gather_cpu_compat(std::ostream& response, const std::string& static_labels);
extern void gather_diskadapters(std::ostream& response, const std::string& static_labels);
extern void gather_diskpaths(std::ostream& response, const std::string& static_labels);
extern void gather_memory_pages(std::ostream& response, const std::string& static_labels);
extern void gather_memory(std::ostream& response, const std::string& static_labels);
extern void gather_cpus(std::ostream& response, const std::string& static_labels);
extern void gather_disks(std::ostream& response, const std::string& static_labels);
extern void gather_netinterfaces(std::ostream& response, const std::string& static_labels);
extern void gather_netadapters(std::ostream& response, const std::string& static_labels);
extern void gather_netbuffers(std::ostream& response, const std::string& static_labels);
extern void gather_partition(std::ostream& response, const std::string& static_labels);
extern void gather_filesystems(std::ostream& response, const std::string& static_labels);
extern void gather_vmstat_v(std::ostream& response, const std::string& static_labels);
extern void gather_fcstats(std::ostream& response, const std::string& static_labels);
extern void gather_cpu_utils(std::ostream& response, const std::string& static_labels);
extern void gather_process(std::ostream& response, const std::string& static_labels);



struct mountpoint {
	std::string mountpoint;
	std::string device;
	std::string fstype;
};

struct filesystem {
	std::string mountpoint;
	std::string device;
	std::string fstype;
	u_int64 avail_bytes; // f_bavail
	u_int64 size_bytes; // f_bfrsize
	u_int64 free_bytes; // f_bfree
	u_int64 files; // f_files
	u_int64 files_free; // f_ffree
	u_int64 files_avail; // f_favail
};

extern std::vector<mountpoint> list_mounts();
extern std::vector<filesystem> stat_filesystems(std::vector<mountpoint> mounts);
