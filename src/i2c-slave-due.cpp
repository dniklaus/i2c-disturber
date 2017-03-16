/*
 * i2c-slave-due.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */

#include <Arduino.h>
#include <Wire.h>
#include <string.h>
// PlatformIO libraries
#include <SerialCommand.h>  // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand

// private libraries
#include <Timer.h>
#include <DbgCliNode.h>
#include <DbgCliTopic.h>
#include <DbgCliCommand.h>
#include <DbgTracePort.h>
#include <DbgTraceContext.h>
#include <DbgTraceOut.h>
#include <DbgPrintConsole.h>
#include <DbgTraceLevel.h>
#include <ProductDebug.h>
#include <RamUtils.h>

SerialCommand* sCmd = 0;

const uint32_t pinSCL = 21;
const uint32_t pinSDA = 20;

void i2cStart()
{
  // SDA: high to low while SCL is high
  pinMode(pinSDA, OUTPUT);
  digitalWrite(pinSDA, LOW);

  pinMode(pinSCL, OUTPUT);
  digitalWrite(pinSCL, LOW);

  delayMicroseconds(2);

  digitalWrite(pinSCL, HIGH);
  pinMode(pinSCL, INPUT_PULLUP);

  pinMode(pinSCL, OUTPUT);
  digitalWrite(pinSCL, LOW);

  digitalWrite(pinSDA, HIGH);
  pinMode(pinSDA, INPUT_PULLUP);

  digitalWrite(pinSCL, HIGH);
  pinMode(pinSCL, INPUT_PULLUP);
}

void i2cStop()
{
  // SDA: low to high while SCL is high
  pinMode(pinSCL, OUTPUT);
  digitalWrite(pinSCL, LOW);

  pinMode(pinSDA, OUTPUT);
  digitalWrite(pinSDA, LOW);

  digitalWrite(pinSCL, HIGH);
  pinMode(pinSCL, INPUT_PULLUP);

  digitalWrite(pinSDA, HIGH);
  pinMode(pinSDA, INPUT_PULLUP);
}

//-----------------------------------------------------------------------------

void runRandomSequence();

Timer* randomSequenceTimer = 0;

class RandomSequence_TimerAdapter : public TimerAdapter
{
public:
  void timeExpired()
  {
    runRandomSequence();
  }
};

void doRandomStartOrStop()
{
  if (0 == random(2))
  {
    i2cStop();
  }
  else
  {
    i2cStart();
  }
}

void runRandomSequence()
{
  if (0 != randomSequenceTimer)
  {
    randomSequenceTimer->startTimer(random(5, 10));
  }

  doRandomStartOrStop();
}

//-----------------------------------------------------------------------------

class DbgCli_Cmd_I2cStart : public DbgCli_Command
{
public:
  DbgCli_Cmd_I2cStart(DbgCli_Topic* i2cTopic)
  : DbgCli_Command(i2cTopic, "start", "Send START condition to I2C bus.")
  { }

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
  {
    if (argc - idxToFirstArgToHandle > 0)
    {
      printUsage();
    }
    else
    {
      i2cStart();
    }
  }

  void printUsage()
  {
    Serial.println(getHelpText());
    Serial.println("Usage: dbg i2c start");
  }
};

//-----------------------------------------------------------------------------

class DbgCli_Cmd_I2cStop : public DbgCli_Command
{
public:
  DbgCli_Cmd_I2cStop(DbgCli_Topic* i2cTopic)
  : DbgCli_Command(i2cTopic, "stop", "Send STOP condition to I2C bus.")
  { }

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
  {
    if (argc - idxToFirstArgToHandle > 0)
    {
      printUsage();
    }
    else
    {
      i2cStop();
    }
  }

  void printUsage()
  {
    Serial.println(getHelpText());
    Serial.println("Usage: dbg i2c stop");
  }
};

//-----------------------------------------------------------------------------

class DbgCli_Cmd_I2cSeqRun : public DbgCli_Command
{
public:
  DbgCli_Cmd_I2cSeqRun(DbgCli_Topic* i2cSeqTopic)
  : DbgCli_Command(i2cSeqTopic, "run", "Run random sequence of START and STOP conditions sent to I2C bus.")
  { }

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
  {
    if (argc - idxToFirstArgToHandle > 0)
    {
      printUsage();
    }
    else
    {
      runRandomSequence();
    }
  }

  void printUsage()
  {
    Serial.println(getHelpText());
    Serial.println("Usage: dbg i2c seq run");
  }
};

class DbgCli_Cmd_I2cSeqQuit : public DbgCli_Command
{
public:
  DbgCli_Cmd_I2cSeqQuit(DbgCli_Topic* i2cSeqTopic)
  : DbgCli_Command(i2cSeqTopic, "quit", "Quit running random sequence of START and STOP conditions sent to I2C bus.")
  { }

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
  {
    if (argc - idxToFirstArgToHandle > 0)
    {
      printUsage();
    }
    else
    {
      if (0 != randomSequenceTimer)
      {
        randomSequenceTimer->cancelTimer();
      }
    }
  }

  void printUsage()
  {
    Serial.println(getHelpText());
    Serial.println("Usage: dbg i2c seq quit");
  }
};

class DbgCli_Cmd_I2cSeqStat : public DbgCli_Command
{
public:
  DbgCli_Cmd_I2cSeqStat(DbgCli_Topic* i2cSeqTopic)
  : DbgCli_Command(i2cSeqTopic, "stat", "Show random sequence of START and STOP conditions sent to I2C bus run status.")
  { }

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
  {
    if (argc - idxToFirstArgToHandle > 0)
    {
      printUsage();
    }
    else
    {
      if (0 != randomSequenceTimer)
      {
        Serial.print("Random sequence of START and STOP conditions is ");
        if (!randomSequenceTimer->isRunning())
        {
          Serial.print("not ");
        }
        Serial.println("running");
      }
    }
  }

  void printUsage()
  {
    Serial.println(getHelpText());
    Serial.println("Usage: dbg i2c seq stat");
  }
};

//-----------------------------------------------------------------------------

void setup()
{
  setupDebugEnv();

  randomSeed(analogRead(0));
  randomSequenceTimer = new Timer(new RandomSequence_TimerAdapter(), Timer::IS_NON_RECURRING);

  pinMode(pinSCL, INPUT_PULLUP);
  pinMode(pinSDA, INPUT_PULLUP);

  DbgCli_Topic* i2cTopic = new DbgCli_Topic(DbgCli_Node::RootNode(), "i2c", "I2C debug commands");
  new DbgCli_Cmd_I2cStart(i2cTopic);
  new DbgCli_Cmd_I2cStop(i2cTopic);
  DbgCli_Topic* i2cSeqTopic = new DbgCli_Topic(i2cTopic, "seq", "I2C START / STOP sequences commands");
  new DbgCli_Cmd_I2cSeqRun(i2cSeqTopic);
  new DbgCli_Cmd_I2cSeqQuit(i2cSeqTopic);
  new DbgCli_Cmd_I2cSeqStat(i2cSeqTopic);
}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  yield();                  // process Timers
}
