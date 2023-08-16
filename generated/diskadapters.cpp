#include <iostream>

void output_diskadapter_stat_mode(std::ostream& response, const std::string& static_labels, const std::string& name, const std::string& type, const std::string& help, perfstat_diskadapter_t diskadapters[], size_t diskadapter_count, const std::function<double (perfstat_diskadapter_t&)>& func) {
	response << "# HELP " << name << " " << help << std::endl;
	response << "# TYPE " << name << " " << type << std::endl;


	for(size_t i=0; i<diskadapter_count; i++) {
		response << name << "{diskadapter=\"" << diskadapters[i].name << "\",";
        response << static_labels << "} " << func(diskadapters[i]) << std::endl;
	}
}

void gather_diskadapters(std::ostream& response, const std::string& static_labels) {

	int diskadapter_count = perfstat_diskadapter(NULL, NULL, sizeof(perfstat_diskadapter_t), 0);
    if (diskadapter_count == -1) {
        std::cout << "diskadapter count returned error " << errno << std::endl;
        perror("Error calling diskadapter");
        return;
    }
	perfstat_diskadapter_t *diskadapters = new perfstat_diskadapter_t[diskadapter_count];
	perfstat_id_t firstdiskadapter;

	strcpy(firstdiskadapter.name, FIRST_DISKADAPTER);

	if(perfstat_diskadapter(&firstdiskadapter, diskadapters, sizeof(perfstat_diskadapter_t), diskadapter_count) <= 0) {
		perror("Error calling perfstat_diskadapter");
		delete [] diskadapters;
		return;
	}


	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_number", "gauge", "number of disks connected to adapter", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.number; });
	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_size", "gauge", "total size of all disks (in MB)", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.size; });
	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_free", "gauge", "free portion of all disks (in MB)", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.free; });
	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_xrate", "counter", "__rxfers: total number of reads via adapter", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.xrate; });
	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_xfers", "counter", "total number of transfers via adapter", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.xfers; });
	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_rblks", "counter", "512 bytes blocks written via adapter", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.rblks; });
	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_wblks", "counter", "512 bytes blocks read via adapter", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.wblks; });
	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_time", "counter", "amount of time disks are active", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.time; });
	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_adapter_type", "gauge", "0 ==> SCSI, SAS, other legacy adapter types 1 ==> Virtual SCSI/SAS Adapter 2 ==> Fiber Channel Adapter", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.adapter_type; });
	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_dk_bsize", "gauge", "Number of Bytes in a block for this disk", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.dk_bsize; });
	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_dk_rxfers", "counter", "Number of  transfers from disk", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.dk_rxfers; });
	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_dk_rserv", "counter", "read or receive service time", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.dk_rserv; });
	output_diskadapter_stat_mode(response, static_labels, "aix_diskadapter_dk_wserv", "counter", "write or send service time", diskadapters, diskadapter_count, [](perfstat_diskadapter_t& diskadapter) { return diskadapter.dk_wserv; });
	delete [] diskadapters;
}
