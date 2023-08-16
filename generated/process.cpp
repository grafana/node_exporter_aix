#include <iostream>

void output_process_stat_mode(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_process_t process[], size_t process_count, const std::function<double (perfstat_process_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;

	for(size_t i=0; i < process_count; i++) {
		response << name << "{name=\"" << process[i].proc_name << "\",";
        response <<  static_labels << "} " << func(process[i]) << std::endl;
	}
}

void gather_process(std::ostream& response, const std::string& static_labels) {
  perfstat_id_t id;
  int rc,proc_count;

  /* Get the count of processes */
  proc_count = perfstat_process(NULL, NULL,sizeof(perfstat_process_t),0);

  /* check for error */
  if(proc_count <= 0)
  {
    perror("Error in perfstat_process");
    return;
  }
  printf("Number of Processes = %d\n",proc_count);

  /* Allocate enough memory */
  perfstat_process_t *proct = new perfstat_process_t[proc_count];
  if(proct == NULL)
  {
    perror("Memory Allocation Error");
    return;
  }

  strcpy(id.name,"");
  rc = perfstat_process(&id,proct,sizeof(perfstat_process_t),proc_count); 
  if(rc <= 0) 
  { 
    perror("Error in perfstat_process"); 
    return;
  }

    output_process_stat_mode(response,static_labels,"aix_process_num_threads","gauge","thread count",proct,proc_count,[](perfstat_process_t& p){return p.num_threads;});
    output_process_stat_mode(response,static_labels,"aix_process_virtual_size","gauge","virtual size of process",proct,proc_count,[](perfstat_process_t& p){return p.proc_size;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_real_data","gauge","real memory used for the data in kilobytes",proct,proc_count,[](perfstat_process_t& p){return p.proc_real_mem_data;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_real_text","gauge","real memory used for the text in kilobytes",proct,proc_count,[](perfstat_process_t& p){return p.proc_real_mem_text;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_virtual_data","gauge","virtual memory used for the data in kilobytes",proct,proc_count,[](perfstat_process_t& p){return p.proc_virt_mem_data;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_virtual_text","gauge","virtual memory used for the text in kilobytes",proct,proc_count,[](perfstat_process_t& p){return p.proc_virt_mem_text;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_shared","gauge","data size from shared libary in kilobytes",proct,proc_count,[](perfstat_process_t& p){return p.shared_lib_data_size;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_heap_size","gauge","heap size in kilobytes",proct,proc_count,[](perfstat_process_t& p){return p.heap_size;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_real_inuse","gauge","real memory in kilobytes used by the process including segments",proct,proc_count,[](perfstat_process_t& p){return p.real_inuse;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_virtual_inuse","gauge","virtual memory in kilobytes used by the process including segments",proct,proc_count,[](perfstat_process_t& p){return p.virt_inuse;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_pinned","gauge","pinned memory in kilobytes used for the process that is inclusive of all segments",proct,proc_count,[](perfstat_process_t& p){return p.pinned;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_paging","gauge","paging space in kilobytes used for the process that is inclusive of all segments",proct,proc_count,[](perfstat_process_t& p){return p.pgsp_inuse;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_file_pages","gauge","file pages in kilobytes used including shared pages",proct,proc_count,[](perfstat_process_t& p){return p.filepages;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_real_inuse_map","gauge","real memory in kilobytes used for shared memeory and mapped regions",proct,proc_count,[](perfstat_process_t& p){return p.real_inuse_map;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_virtual_inuse_map","gauge","virtual memory in kilobytes used for shared memory and memory mapped regions",proct,proc_count,[](perfstat_process_t& p){return p.virt_inuse_map;});
    output_process_stat_mode(response,static_labels,"aix_process_mem_pinned_inuse_map","gauge","pinned memory in kilobytes for shared memory and memory mapped regions",proct,proc_count,[](perfstat_process_t& p){return p.pinned_inuse_map;});
    output_process_stat_mode(response,static_labels,"aix_process_cpu_user_time","counter","user mode cpu time in milliseconds",proct,proc_count,[](perfstat_process_t& p){return p.ucpu_time;});
    output_process_stat_mode(response,static_labels,"aix_process_cpu_system_time","counter","system mode cpu time in milliseconds",proct,proc_count,[](perfstat_process_t& p){return p.scpu_time;});
    output_process_stat_mode(response,static_labels,"aix_process_timebase","counter","timebase counter",proct,proc_count,[](perfstat_process_t& p){return p.last_timebase;});
    output_process_stat_mode(response,static_labels,"aix_process_disk_read","gauge","bytes read from disk",proct,proc_count,[](perfstat_process_t& p){return p.inBytes;});
    output_process_stat_mode(response,static_labels,"aix_process_disk_write","gauge","bytes written to the disk",proct,proc_count,[](perfstat_process_t& p){return p.outBytes;});
    output_process_stat_mode(response,static_labels,"aix_process_disk_write_ops","gauge","write operations from disk",proct,proc_count,[](perfstat_process_t& p){return p.outOps;});
    output_process_stat_mode(response,static_labels,"aix_process_disk_read_ops","gauge","read operations from disk",proct,proc_count,[](perfstat_process_t& p){return p.inOps;});
    output_process_stat_mode(response,static_labels,"aix_process_num_threads","gauge","thread count",proct,proc_count,[](perfstat_process_t& p){return p.num_threads;});

    delete [] proct;
}
