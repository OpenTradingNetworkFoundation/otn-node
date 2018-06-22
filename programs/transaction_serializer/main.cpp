#include <iostream>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <boost/program_options.hpp>

#include <fc/io/json.hpp>
#include <fc/filesystem.hpp>
#include <fc/crypto/hex.hpp>
#include <graphene/chain/protocol/transaction.hpp>
#include <graphene/chain/protocol/protocol.hpp>
#include <graphene/chain/protocol/fee_schedule.hpp>

#include <graphene/chain/account_object.hpp>
#include <graphene/chain/asset_object.hpp>
#include <graphene/chain/balance_object.hpp>
#include <graphene/chain/committee_member_object.hpp>
#include <graphene/chain/confidential_object.hpp>
#include <graphene/chain/fba_object.hpp>
#include <graphene/chain/market_object.hpp>
#include <graphene/chain/proposal_object.hpp>
#include <graphene/chain/vesting_balance_object.hpp>
#include <graphene/chain/withdraw_permission_object.hpp>
#include <graphene/chain/witness_object.hpp>
#include <graphene/chain/worker_object.hpp>
#include <graphene/utilities/git_revision.hpp>

const char* default_infile  = "transaction.json";
namespace bpo = boost::program_options;
int main( int argc, char** argv )
{
   try {
      boost::program_options::options_description opts;
         opts.add_options()
         ("help,h"   ,                                                             "Print this help message and exit.")
         ("input,i"  , bpo::value<std::string>()->implicit_value(default_infile) , "Input file with transaction JSON")
         ("output,o" , bpo::value<std::string>()                                 , "Output binary file (if empty, show hex representation on stdout)")
         ("version,v",                                                             "Display version information");
      bpo::variables_map options;

      bpo::store( bpo::parse_command_line(argc, argv, opts), options );

      if( options.count("help") ) {
         std::cout << opts << "\n";
         return EXIT_SUCCESS;
      }
      if( options.count("version") ) {
         std::cout << "Version: " << graphene::utilities::git_revision_description << "\n";
         std::cout << "SHA: " << graphene::utilities::git_revision_sha << "\n";
         std::cout << "Timestamp: " << fc::get_approximate_relative_time_string(fc::time_point_sec(graphene::utilities::git_revision_unix_timestamp)) << "\n";
         return EXIT_SUCCESS;
      }
      std::string input(options.count("input")? options.at("input").as<std::string>() : default_infile);
      if( ! fc::exists(input) ) {
         std::cerr << "Unable to open " << input << std::endl;
         return EXIT_FAILURE;
      }
      graphene::chain::transaction tx(fc::json::from_file(input).as< graphene::chain::transaction >());

      auto data(fc::raw::pack(tx));
      if( ! options.count("output")) {
         std::string hex_representation(fc::to_hex(data));
         std::cout << hex_representation << std::endl;
         return EXIT_SUCCESS;
      }
      std::string output(options.at("output").as<std::string>());
      std::ofstream of(output, std::ios::out | std::ios::trunc | std::ios::binary);
      if( ! of.is_open()) {
         std::cerr << "Unable to open file '" << output << "'!: " << strerror(errno) << std::endl;
         return EXIT_FAILURE;
      }
      of.write(&data[0], data.size() * sizeof(data[0]));
      of.close();
   } catch ( const fc::exception& e ) {
      edump((e.to_detail_string()));
   }
   return EXIT_SUCCESS;
}
