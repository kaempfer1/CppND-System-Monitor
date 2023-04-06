#include <string>

#include "format.h"

using std::string;
//==
using std::to_string;
//==

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

//string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }
//==
string Format::ElapsedTime(long seconds) {

  int h, m, s;									// temp variables to calculate hour, min, sec values
  string str_h, str_m, str_s;					// temp strings to build the HH:MM:SS
  
  h = seconds / (60 * 60);						// get hours by dividing total by # of seconds in an hour (60min/hr * 60sec/min)
  m = seconds%(60 * 60) / 60; 					// get the minutes. remove full hours from total seconds (60sec/min)
  s = seconds%(60 * 60) % 60;					// get the seconds. remove full hours and minutes
  
  if (h < 10) { str_h = "0" + to_string(h); }	// build the hour HH string. Add leading 0 if needed
  else { str_h = to_string(h); }

  if (m < 10) { str_m = "0" + to_string(m); }	// build the minute MM string
  else { str_m = to_string(m); }

  if (s < 10) { str_s = "0" + to_string(s); }	// build the seconds SS string
  else { str_s = to_string(s); }
  
  return (str_h + ":" + str_m + ":" + str_s);  // return the full HH:MM:SS string
}
//==