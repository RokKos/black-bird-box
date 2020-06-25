#include "bbbpch.h"
#include "Instrumentator.h"

#include "BlackBirdBox/Core/Macros.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"


namespace Core {
	

	Instrumentor::Instrumentor()
		: current_session_(nullptr)
	{
	}

	void Instrumentor::BeginSession(const std::string& filepath)
	{
		if (current_session_)
		{
			// If there is already a current session, then close it before beginning new one.
			// Subsequent profiling output meant for the original session will end up in the
			// newly opened session instead.  That's better than having badly formatted
			// profiling output.
			if (Log::GetLogger()) // Edge case: BeginSession() might be before Log::Init()
			{
				LOG_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", filepath, current_session_->FilePath);
			}
			InternalEndSession();
		}
		
		output_json_.SetObject();

		current_session_ = new InstrumentationSession({ filepath });
		WriteHeader();

	}

	void Instrumentor::EndSession()
	{
		InternalEndSession();
	}

	void Instrumentor::WriteProfile(const ProfileResult& result)
	{
		rapidjson::Value trace_events = output_json_["traceEvents"].GetArray();

		rapidjson::Value traced_event(rapidjson::Type::kObjectType);
		traced_event.AddMember("cat", "function", output_json_.GetAllocator());
		traced_event.AddMember("dur", result.ElapsedTime.count(), output_json_.GetAllocator());
		
		rapidjson::Value event_name(rapidjson::Type::kStringType);
		event_name.SetString(result.Name.c_str(), result.Name.length(), output_json_.GetAllocator());
		traced_event.AddMember("name", event_name, output_json_.GetAllocator());

		traced_event.AddMember("ph", "X", output_json_.GetAllocator());
		traced_event.AddMember("pid", "0", output_json_.GetAllocator());

		rapidjson::Value event_thread(rapidjson::Type::kStringType);
		std::stringstream thread_id;
		thread_id << result.ThreadID;
		event_thread.SetString(thread_id.str().c_str(), thread_id.str().length(), output_json_.GetAllocator());
		traced_event.AddMember("tid", event_thread, output_json_.GetAllocator());

		traced_event.AddMember("ts", result.Start.count(), output_json_.GetAllocator());

		trace_events.PushBack(traced_event, output_json_.GetAllocator());
		output_json_.AddMember("traceEvents", trace_events, output_json_.GetAllocator());
	}

	void Instrumentor::WriteHeader()
	{
		rapidjson::Value other_data(rapidjson::Type::kObjectType);
		output_json_.AddMember("otherData", other_data, output_json_.GetAllocator());

		rapidjson::Value trace_events(rapidjson::Type::kArrayType);
		output_json_.AddMember("traceEvents", trace_events, output_json_.GetAllocator());

	}

	void Instrumentor::InternalEndSession()
	{
		if (current_session_)
		{
			rapidjson::StringBuffer strbuf;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
			output_json_.Accept(writer);

			std::ofstream output_file(current_session_->FilePath);
			output_file << strbuf.GetString();
			if (!output_file.good()) {
				CORE_ASSERT("Instrumentor could not write the path: {0} JSON string to the file!", current_session_->FilePath);
			}

			delete current_session_;
			current_session_ = nullptr;
		}
	}

}