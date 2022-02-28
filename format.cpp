#include <string>
#include <algorithm>  
#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long times) 
{
 long seconds = times % 60 ;
 long minutes = (times /60 ) % 60 ; 
 long hours = times / 3600;
 string hourformat = std::to_string(hours);
 string minuteformat = std::to_string(minutes);
 string secondformat = std::to_string(seconds);
 if(hourformat.size() == 1)
 {
  hourformat.append("0");
  reverse(hourformat.begin(), hourformat.end()); 
 }
 if(minuteformat.size() == 1)
 {
  minuteformat.append("0");
  reverse(minuteformat.begin(), minuteformat.end()); 
 }
 if(secondformat.size() == 1)
 {
  secondformat.append("0");
  reverse(secondformat.begin(), secondformat.end()); 
 }  
 string timeformat = hourformat   + ":" +
                     minuteformat + ":" +
                     secondformat;
  return timeformat;
}