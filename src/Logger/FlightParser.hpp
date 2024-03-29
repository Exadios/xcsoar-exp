// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

struct FlightInfo;
struct BrokenDateTime;
class NLineReader;

/**
 * A parser for files generated by #FlightLogger.
 */
class FlightParser {
  NLineReader &reader;

  char *last;
  char *current_line;

public:
  FlightParser(NLineReader &_reader):reader(_reader), last(nullptr) {}

  /**
   * Read the next flight.  Returns true if the data has been written
   * to #flight, false on end-of-file or error.
   */
  bool Read(FlightInfo &flight);

private:
  char *ReadLine();
  char *ReadLine(BrokenDateTime &dt);
};
