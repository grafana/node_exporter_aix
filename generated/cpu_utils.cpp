#include <iostream>
#include <math.h>

void output_cpu_util_stat_mode(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_cpu_util_t cpu_utils[], size_t cpu_util_count, const std::function<double (perfstat_cpu_util_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;


    std::cout << "cpu_util getting name " <<  name  << std::endl;
	for(size_t i=0; i<cpu_util_count; i++) {
	    double value = func(cpu_utils[i]);
	    // This detects NAN
	    if (isnan(value))
	    {
            continue;
	    }

        std::cout << "cpu_util index " <<  i  << std::endl;
		response << name << "{id=\"" << cpu_utils[i].cpu_id << "\",";
        response <<  static_labels << "} " << value << std::endl;
	}
}

void gather_cpu_utils(std::ostream& response, const std::string& static_labels) {


    std::cout << static_labels  << std::endl;
    std::cout << "cpu_util starting "  << std::endl;
    perfstat_id_t id;
    perfstat_rawdata_t data;
    int cpu_count,rc;

    /* Check how many perfstat_cpu_t structures are available */
    cpu_count = perfstat_cpu(NULL, NULL,sizeof(perfstat_cpu_t),0);

    /* check for error */
    if(cpu_count <= 0)
    {
        perror("Error in perfstat_cpu");
        return;
    }
    /* allocate enough memory */
    perfstat_cpu_t *oldt = new perfstat_cpu_t[cpu_count];
    if(oldt == NULL)
    {
        perror("Memory Allocation Error");
        return;
    }
    /* set name to first cpu */
    strcpy(id.name,FIRST_CPU);
    /* ask to get all the structures available in one call */
    rc = perfstat_cpu(&id, oldt, sizeof(perfstat_cpu_t), cpu_count);
    /* check for error */
    if(rc <=0)
    {
        perror("Error in perfstat_cpu");
        return;
    }
    data.type = UTIL_CPU;
    data.prevstat= oldt;
    data.sizeof_data = sizeof(perfstat_cpu_t);
    data.prev_elems = cpu_count;
    sleep(1);
    /* Check how many perfstat_cpu_t structures are available after a defined period */
    cpu_count = perfstat_cpu(NULL, NULL,sizeof(perfstat_cpu_t),0);

    /* Check for error */
    if(cpu_count <= 0)
    {
        perror("Error in perfstat_cpu");
        return;
    }
    // We have to sample the CPU
    data.cur_elems = cpu_count;
    if(data.prev_elems != data.cur_elems)
    {
        perror("The number of CPUs has become different for defined period");
        return;
    }
    /* allocate enough memory */
    perfstat_cpu_t *newt = new perfstat_cpu_t[cpu_count];
    perfstat_cpu_util_t *cpu_utils = new perfstat_cpu_util_t[cpu_count];
    if(newt == NULL || cpu_utils == NULL)
    {
        perror("Memory Allocation Error");
        return;
    }
    data.curstat = newt;
    rc = perfstat_cpu(&id, newt, sizeof(perfstat_cpu_t), cpu_count);
    if(rc <= 0)
    {
        perror("Error in perfstat_cpu");
        return;
    }
    /* Calculate CPU Utilization Metrics*/
    rc = perfstat_cpu_util(&data, cpu_utils, sizeof(perfstat_cpu_util_t), cpu_count);
    if(rc <= 0)
    {
        perror("Error in perfstat_cpu_util");
        return;
    }

    output_cpu_util_stat_mode(response, static_labels, "aix_cpu_util_physical_busy", "gauge", "physical processors busy.", cpu_utils, cpu_count, [](perfstat_cpu_util_t& cpu_util) { return cpu_util.physical_busy; });
    output_cpu_util_stat_mode(response, static_labels, "aix_cpu_util_physical_consumed", "gauge", "total processors used up by the partition", cpu_utils, cpu_count, [](perfstat_cpu_util_t& cpu_util) { return cpu_util.physical_consumed; });


    std::cout << "delete oldt"  << std::endl;
    delete [] oldt;

    std::cout << "delete newt"  << std::endl;
    delete [] newt;

    std::cout << "delete cpu_utils"  << std::endl;
    delete [] cpu_utils;

    std::cout << "done"  << std::endl;
}
