#ifndef __REDUCER_ACCEL_H__
#define __REDUCER_ACCEL_H__
#ifdef HW_ACCEL

#define HW_MAXIMUM_SOURCES 32

#include <mutex>
#include <vector>
#include <tuple>
#include <chrono>

#include "reducer.h"

#include "alignedbuffermanager.h"
#include "tempfilemanager.h"
#include "types.h"
#include "utils.h"

#include "bdbmpcie.h"

/**
TODO: only allow ONE instance
**/

namespace SortReduceReducer {
	template <class K, class V>
	class MergerNodeAccel : public BlockSource<K,V> {
	public:
		MergerNodeAccel(V (*update)(V,V) = NULL); 
		~MergerNodeAccel();
		void AddSource(BlockSource<K,V>* src);
		bool Start();
		
		SortReduceTypes::Block GetBlock();
		void ReturnBlock(SortReduceTypes::Block block);

	private:
		bool SendReadBlock(BlockSource<K,V>* src, int idx);
		void SendReadBlockDone(int idx);
		void SendWriteBlock();
		void EmitBlock(size_t offset, size_t bytes, bool last);
	public:
		static bool InstanceExist() {return m_instance_exist;};
		static int MaxSources() { return m_max_sources; };
	private:
		static bool m_instance_exist;
		static const int m_max_sources = 32;
	private:
		std::thread m_worker_thread;
		void WorkerThread();
		bool m_started;
		bool m_kill;

		std::mutex m_mutex;



		int m_source_count;
		BlockSource<K,V>* ma_sources[HW_MAXIMUM_SOURCES];
		size_t ma_sources_offset[HW_MAXIMUM_SOURCES];
		SortReduceTypes::Block ma_cur_read_blocks[HW_MAXIMUM_SOURCES];
		std::queue<int> maq_read_buffers_inflight[HW_MAXIMUM_SOURCES];
		
		bool ma_last_sent[HW_MAXIMUM_SOURCES];

		V (*mp_update)(V,V) = NULL;

		std::queue<int> mq_free_dma_idx; // for host->fpga
		static const int m_write_buffer_idx_start = 128;
		static const int m_write_buffer_idx_end = 256;
		int m_cur_write_buffer_idx;
		int m_write_buffers_inflight = 0;
		
		// Output stuff
		std::queue<int> mq_ready_idx;
		std::queue<int> mq_free_idx;
		std::vector<SortReduceTypes::Block> ma_blocks;
		int m_cur_out_idx;
		size_t m_out_offset;
		size_t m_total_out_bytes;
		SortReduceTypes::Block m_cur_out_block;
	};
}

#endif // HW_ACCEL
#endif // ifndef __REDUCER_ACCEL__
