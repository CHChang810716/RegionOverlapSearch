#include <overlap_search.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
struct RegionEnd {
    using Id = std::uint64_t;
    Id id;
    std::uint64_t value;
    bool operator<( const RegionEnd& e ) {
        if ( value == e.value ) {
            return ( id < e.id );
        } else { 
            // std::clog << value << " < " << e.value << " ? " << std::endl;
            // std::clog << (value < e.value ) << std::endl;
            return ( value < e.value ); 
        }
    }
};

int main(int argc, const char* argv[] ) {
    if ( argc < 4 ) {
        std::cout << "Usage: super_enhancer_cluster [file path] [extension length] [output path]" << std::endl;
        std::exit(1);
    }

    OverlapSearch<RegionEnd> os;
    std::int32_t ext_len = std::stoi(argv[2]) / 2;
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[3]);
    std::string line;
    typename RegionEnd::Id region_id = 0;
    while( std::getline(fin, line) ) {
        auto fline ( split(line, '\t') );
        std::string& chr(fline[0]);
        std::string& strand(fline[1]);
        RegionEnd start { region_id, std::stoi(fline[2]) - ext_len};
        RegionEnd end { region_id, std::stoi(fline[3]) + ext_len};
        os.push(chr /* + strand (ignore strand) */, start );
        os.push(chr /* + strand (ignore strand) */, end );
        region_id ++;
    }
    fout << "cluster_id\tchr\tstart\tend\tregion_id" << std::endl;
    os([ext_len, &fout](int cid, const std::string& label, const RegionEnd& start, const RegionEnd& end) -> void{
        fout << cid << '\t' << label << '\t' << start.value + ext_len << '\t' << end.value - ext_len << '\t' << start.id << std::endl;
    });
}