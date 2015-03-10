//
//#include <sookee/types.h>
////#include <sookee/str.h>
//#include <sookee/bug.h>
//#include <sookee/log.h>
//#include <sookee/test.h>
////#include <sookee/i18n.h>
//#include <limits>
//#include <iomanip>
//#include <array>
//#include <bitset>
//
////#include <unicode/utypes.h>
////#include <unicode/ucnv.h>
////#include <unicode/uchar.h>
//
//#include <mutex>
//#include <future>
//#include <atomic>
//#include <condition_variable>
//
//using namespace sookee::types;
//using namespace sookee::bug;
//using namespace sookee::log;
////using namespace sookee::i18n;
//
//namespace sookee { namespace thread {
//
//template<typename Type>
//class procon
//{
//public:
////	typedef int Type;
//	typedef std::function<void(Type const&)> Consumer;
//	typedef std::function<Type()> Producer;
//
//private:
//	std::mutex mtx;
//	std::condition_variable cnd;
//	std::atomic_bool done;
//
//	std::queue<Type> q;
//
//public:
//	procon(): done(false) {}
//	procon(procon&& pc): mtx(pc.mtx), cnd(pc.cnd), done(pc.done), q(pc.q) {}
//	procon(const procon& pc) = delete;
//
////	template<typename Consumer>
//	void consume(Consumer consumer)
//	{
//		unique_lock lock(mtx);
//
//		cnd.wait(lock, [this](){ return !q.empty(); });
//
//		while(!q.empty())
//		{
//			consumer(q.front());
//			q.pop();
//		}
//	}
//
//	void produce(Producer producer)
//	{
//		{
//			unique_lock lock(mtx);
//			q.push(producer());
//		}
//		cnd.notify_one();
//	}
//
//	bool extract(Type& type)
//	{
//		unique_lock lock(mtx);
//
//		cnd.wait(lock, [this](){ return !q.empty(); });
//
//		if(!q.empty())
//		{
//			type = q.front();
//			q.pop();
//			return true;
//		}
//		return false;
//	}
//
//	void insert(const Type& type)
//	{
//		{
//			unique_lock lock(mtx);
//			q.push(type);
//		}
//		cnd.notify_one();
//	}
//
//	void run(Consumer consumer)
//	{
////		bug_func();
//		while(!done)
//			consume(consumer);
//	}
//
//	void run(Producer producer)
//	{
////		bug_func();
//		while(!done)
//			produce(producer);
//	}
//
//	void stop()
//	{
//		done = true;
//	}
//};
//
//} // thread
//
//namespace log {
//
////typedef std::unique_ptr<sss> sss_uptr;
////typedef sss* sss*;
//
//inline std::string get_stamp()
//{
//	time_t rawtime = std::time(0);
//	tm* timeinfo = std::localtime(&rawtime);
//	char buffer[32];
//	std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", timeinfo);
//
//	return std::string(buffer);
//}
//
//class writer
//{
//	friend class mapper;
//
//	struct message
//	{
//		std::string stamp;
//		sss* stream;
//	};
//
//	thread::procon<message> pc;
//
//	void consumer(message& msg)
//	{
//		(*sink) << msg.stamp << " [" << id << "] " << msg.stream->rdbuf() << this->endl << std::flush;
//		delete msg.stream;
//	}
//
//	typedef std::queue<message> message_que;
//	USING_MAP(sos*, message_que, queue_map);
//
//	static std::mutex queues_mtx;
//	static queue_map queues;
//
//	std::string id;
//	std::ostream* sink = nullptr;
//	message_que* q = nullptr;
//
//	std::string endl = "\n";
//
//	writer& set_id(const str& id) { this->id = id; return *this; }
//
//public:
//
//	writer() { set_sink(std::clog); }
//	writer(sos& sink) { set_sink(sink); }
//
//	void set_sink(sos& sink)
//	{
//		queue_map_citer i;
//		lock_guard lock(queues_mtx);
//		if((i = queues.find(&sink)) == queues.cend())
//			queues[&sink];
//		this->q = &queues[&sink];
//		this->sink = &sink;
//	}
//
//	void set_endl(char endl) { this->endl = std::string(1, endl); }
//	void set_endl(const std::string& endl) { this->endl = endl; }
//
//	void add_line(sss* ss)
//	{
////		message msg {get_stamp(), ss};
//		pc.insert({get_stamp(), ss});
//	}
//};
//
//writer::queue_map writer::queues;
//
//struct buffer
//{
//	writer& lw;
//	sss* ss;
//
//	buffer(writer& lw): lw(lw), ss(new sss) {}
//	buffer(const buffer& buf) = delete;
//	buffer(buffer&& buf): lw(buf.lw), ss(buf.ss) { buf.ss = nullptr; }
//	~buffer() { if(ss) lw.add_line(ss); }
//};
//
//template<typename Type>
//buffer operator<<(buffer&& buf, const Type& t)
//{
//	(*buf.ss) << t;
//	return std::move(buf);
//}
//
//template<typename Type>
//buffer operator<<(writer& lw, const Type& t)
//{
//	return std::move(buffer(lw) << t);
//}
//
//class mapper
//{
//	typedef std::map<str,writer> logmap;
//private:
//	static std::recursive_mutex mtx;
//	static logmap logs;
//
//	static writer err;
//
//public:
//	static writer& create(const str& name, std::ostream& os)
//	{
////		bug_func();
//		recursive_lock_guard lock(mtx);
//		if(!logs.count(name))
//			logs.emplace(name, os);
//		else
//			err << "WARN: calling create on existing log_writer: " << name;
//		return get(name);
//	}
//
//	static writer& get(const str& name)
//	{
////		bug_func();
//		recursive_lock_guard lock(mtx);
//		if(!logs.count(name))
//			err << "WARN: calling get on non-existent log_writer, creating null: " << name;
//		return logs[name].set_id(name);
//	}
//};
//
//
//}} // :: sookee::log
//
//using namespace sookee::thread;
//
//int main()
//{
////	std::ios::sync_with_stdio(false);
//
//	procon<int> pc;
//	procon<int>::Producer producer = []{ static int i = 0; return ++i; };
//	procon<int>::Consumer consumer = [](int i){ con(i); };
//
//	std::future<void> cfut = std::async(std::launch::async, [&]{pc.run(consumer);});
//	std::future<void> pfut = std::async(std::launch::async, [&]{pc.run(producer);});
//
//	std::cin.ignore(std::numeric_limits<std::streamsize>::max());
//	std::cin.get();
//
//	std::this_thread::sleep_for(std::chrono::seconds(3));
//
//	pc.stop();
//
//	if(pfut.valid())
//		pfut.get();
//
//	if(cfut.valid())
//		cfut.get();
//
//	return 1;
//
//	std::ofstream ofs("i18n-ws.h");
//
//	ofs << "#ifndef LIBSOOKEE_I18N_H_" << '\n';
//	ofs << "#define LIBSOOKEE_I18N_H_" << '\n';
//	ofs << "// Autogenerated header file" << '\n';
//	ofs << '\n';
//	ofs << "namecpace sookee { namespace i18n {" << '\n';
//	ofs << '\n';
//	ofs << "const char32_t ws[] = " << '\n';
//	ofs << "{" << '\n';
//
//	std::string sep = "\t";
//
//	for(char32_t c = 0; c <= 12288/*std::numeric_limits<char32_t>::max()*/; ++c)
//	{
//		if(u_isUWhiteSpace(c))
//		{
//			std::ostringstream oss;
//			oss << std::hex << std::uppercase << int32_t(c);
//			std::string n = oss.str();
//			if(n.size() < 8)
//				n = std::string(8 - n.size(), '0') + n;
//			ofs << sep << "U'\\U" << n << "'";
//			sep = "\n\t, ";
//		}
//	}
//
//	ofs << sep << "U'\\U00000000'";
//	ofs << "\n};" << '\n';
//	ofs << '\n';
//	ofs << "}} // ::sookee::i18n" << '\n';
//	ofs << '\n';
//	ofs << "#endif // LIBSOOKEE_I18N_H_" << '\n';
//}

int main(){}
