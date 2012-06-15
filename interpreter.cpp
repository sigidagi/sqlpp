#include "interpreter.hpp"


namespace example {

void interpreter::parse_input(std::string const & text) const
{
    boost::char_separator<char> s(" ,()\t\n\r");
    //boost::escaped_list_separator<char> s;

    token_parser parser
      ( boost::make_token_iterator<std::string>(text.begin(), text.end(), s)
      , boost::make_token_iterator<std::string>(text.end()  , text.end(), s) );

    while (parser.has_more_tokens())
    {
      // read function name
      std::string func_name = parser.get<std::string>();

      // look up function
      dictionary::const_iterator entry = map_invokers.find( func_name );
      if (entry == map_invokers.end())
        //throw std::runtime_error("unknown function: " + func_name);
        throw NotAFunction(func_name);

      // call the invoker which controls argument parsing
      entry->second(parser);
    }
}

} // namespace example

