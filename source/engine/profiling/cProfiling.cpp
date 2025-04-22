#include "cProfiling.h"

#ifdef DF_Profiling
	#include <chrono>
	#include <csignal>
	#include <tracy/server/TracyFileWrite.hpp>
	#include <tracy/server/TracyWorker.hpp>

	#include "filesystem/cFileSystem.h"

namespace df
{
	std::atomic< bool > cProfiling::m_running          = false;
	std::thread*        cProfiling::m_profiling_thread = nullptr;

	void cProfiling::start()
	{
		if( m_running )
			return;

		m_running          = true;
		m_profiling_thread = new std::thread( profiling );
	}

	void cProfiling::stop()
	{
		if( !m_running )
			return;

		m_running = false;
		if( m_profiling_thread->joinable() )
			m_profiling_thread->join();

		delete m_profiling_thread;
	}

	void cProfiling::profiling()
	{
		const std::chrono::time_point< std::chrono::system_clock > now   = std::chrono::system_clock::now();
		const long long                                            now_c = std::chrono::system_clock::to_time_t( now );
		std::tm                                                    local_tm;
		localtime_s( &local_tm, &now_c );

		std::ostringstream oss;
		oss << std::put_time( &local_tm, "%Y-%m-%d-%H-%M-%S" );

		const std::string output = filesystem::getGameDirectory() + oss.str() + ".tracy";
		tracy::Worker     worker( "127.0.0.1", 8086, -1 );

		while( !worker.HasData() )
		{
			if( worker.GetHandshakeStatus() == tracy::HandshakeNotAvailable || !m_running )
				return;

			std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
		}

		while( m_running )
			std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

		const std::unique_ptr< tracy::FileWrite > output_file = std::unique_ptr< tracy::FileWrite >( tracy::FileWrite::Open( output.data(), tracy::FileCompression::Zstd, 3, 4 ) );
		if( output_file )
		{
			worker.Write( *output_file, false );
			output_file->Finish();
		}
	}
}
#endif