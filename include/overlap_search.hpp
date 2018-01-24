#pragma once
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
template< class T >
struct OverlapSearch
{
public:
    void push( const std::string& label, const T& entry){
        index_[label].push_back(entry);
    }
    void operator()( const std::function<void(int, const std::string&, const T& t1, const T& t2)>& writer ) {
        sort_index();
        cluster( writer );
    }
private:
    void sort_index() {
        for (auto&& p : index_ ) {
            std::sort(p.second.begin(), p.second.end());
        }
    }
    void cluster( const std::function<void(int, const std::string&, const T& t1, const T& t2)>& writer ) {
        int cluster_id = -1;
        for( auto&& p : index_ ) {
            std::map< typename T::Id, T* > start_pool;
            for (auto&& point : p.second ) {
                if( start_pool.size() == 0 ){ // a new cluster begin
                    cluster_id ++;
                }
                auto s_itr = start_pool.find(point.id);
                if( s_itr == start_pool.end() ) {
                    start_pool.emplace(point.id, &point);
                } else {
                    writer(cluster_id, p.first, *s_itr->second, point);
                    start_pool.erase(s_itr);
                }
            }
        }
    }
    std::map<
        std::string,
        std::vector< T >
    > index_;
};