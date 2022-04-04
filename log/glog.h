#ifndef __LOG_GLOG_H__
#define __LOG_GLOG_H__

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <glog/logging.h>

static void SignalHandle(const char *data, size_t size) {
	//std::ofstreamfs("glog_dump.log", std::ios::app);
	std::string str = std::string(data, size);
	//fs << str;
	//fs.close();
	LOG(ERROR) << str;
}

class GLog {
public:
	GLog(char *program) {

        boost::property_tree::ptree root;
        boost::property_tree::read_json("./json/log.json", root);
		auto logtostderr = root.get<bool>("logtostderr");
		auto alsologtostderr = root.get<bool>("alsologtostderr");
		auto colorlogtostderr = root.get<bool>("colorlogtostderr");
		auto log_prefix = root.get<bool>("log_prefix");
		auto logbufsecs = root.get<int>("logbufsecs");
		auto max_log_size = root.get<int>("max_log_size");
		auto minloglevel = root.get<int>("minloglevel");
		auto stop_logging_if_full_disk = root.get<bool>("stop_logging_if_full_disk");

		// bool logtostderr = true;
		// bool alsologtostderr = true;
		// bool colorlogtostderr = true;
		// bool log_prefix = true;
		// int logbufsecs = 0;
		// int max_log_size = 64;
		// int minloglevel = 0;
		// int stop_logging_if_full_disk = 1;


		google::InitGoogleLogging(program);

		FLAGS_log_dir = "./log/";
		FLAGS_logtostderr = logtostderr;
		FLAGS_alsologtostderr = alsologtostderr;
		FLAGS_colorlogtostderr = colorlogtostderr;
		FLAGS_log_prefix = log_prefix;
		FLAGS_logbufsecs = logbufsecs;
		FLAGS_max_log_size = max_log_size;
		FLAGS_minloglevel = minloglevel;
		FLAGS_stop_logging_if_full_disk = stop_logging_if_full_disk;
		//std::string s("./log/");
		//s += program;
		//s += "_";
		//FLAGS_stderrthreshold = 1;
		//google::FlushLogFiles(google::ERROR);
   		//google::FlushLogFiles(google::GLOG_ERROR);
		//google::SetStderrLogging(google::ERROR);
		//google::SetLogDestination(google::ERROR, s.data());
		//google::SetStderrLogging(google::INFO);
		//google::SetLogDestination(google::ERROR, s.data());
		//google::SetLogDestination(google::INFO, "./log/");

		google::InstallFailureSignalHandler();
		google::InstallFailureWriter(&SignalHandle);
	}

	~GLog() {
		google::ShutdownGoogleLogging();
	}
};

#endif
