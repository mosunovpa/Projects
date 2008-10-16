//***************************************************************************
// Licensed Materials - Property of IBM
//
// Governed under the terms of the International
// License Agreement for Non-Warranted Sample Code.
//
// (C) COPYRIGHT International Business Machines Corp. 1997 - 2002
// All Rights Reserved.
//
// US Government Users Restricted Rights - Use, duplication or
// disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
//***************************************************************************
//
// SOURCE FILE NAME: Util.java
//
// SAMPLE: Utilities for JDBC sample programs
//
//         This sample has 3 classes:
//         1. Data - Display the data in the table
//         2. Db - Connect to or disconnect from the 'sample' database
//         3. JdbcException - Handle Java Exceptions
//
// JAVA 2 CLASSES USED:
//         DriverManager
//         Connection
//         Exception
//
// OUTPUT FILE: None
//***************************************************************************
//
// For more information on the sample programs, see the README file.
//
// For information on developing JDBC applications, see the Application
// Development Guide.
//
// For information on using SQL statements, see the SQL Reference.
//
// For the latest information on programming, compiling, and running DB2
// applications, visit the DB2 application development website at
//     http://www.software.ibm.com/data/db2/udb/ad
//**************************************************************************/

import java.lang.*;
import java.util.*;
import java.sql.*;
import java.math.BigDecimal;

class Data
{
  public static String format(String strData, int finalLen) throws Exception
  {
    String finalStr;
    if (finalLen <= strData.length())
    {
      finalStr = strData.substring(0, finalLen);
    }
    else
    {
      finalStr = strData;
      for (int i = strData.length(); i < finalLen; i++)
      {
        finalStr = finalStr + " ";
      }
    }
    return (finalStr);
  } // format(String, int)

  public static String format(int intData, int finalLen) throws Exception
  {
    String strData = String.valueOf(intData);
    String finalStr;
    if (finalLen <= strData.length())
    {
      finalStr = strData.substring(0, finalLen);
    }
    else
    {
      finalStr = "";
      for (int i = 0; i < finalLen - strData.length(); i++)
      {
        finalStr = finalStr + " ";
      }
      finalStr = finalStr + strData;
    }
    return (finalStr);
  } // format(int, int)

  public static String format(Integer integerData, int finalLen)
  throws Exception
  {
    int intData;
    String finalStr;

    intData = integerData.intValue();
    finalStr = format(intData, finalLen);

    return (finalStr);
  } // format(Integer, int)

  public static String format(double doubData, int precision, int scale)
  throws Exception
  {
    BigDecimal decData = new BigDecimal(doubData);
    decData = decData.setScale(scale, BigDecimal.ROUND_HALF_EVEN);
    String strData = decData.toString();

    // prepare the final string
    int finalLen = precision + 1;
    String finalStr;
    if (finalLen <= strData.length())
    {
      finalStr = strData.substring(0, finalLen);
    }
    else
    {
      finalStr = "";
      for (int i = 0; i < finalLen - strData.length(); i++)
      {
        finalStr = finalStr + " ";
      }
      finalStr = finalStr + strData;
    }

    return (finalStr);
  } // format(double, int, int)

  public static String format(BigDecimal decData, int precision, int scale)
  throws Exception
  {
    decData = decData.setScale(scale, BigDecimal.ROUND_HALF_EVEN);
    String strData = decData.toString();

    // prepare the final string
    int finalLen = precision + 1;
    String finalStr;
    if (finalLen <= strData.length())
    {
      finalStr = strData.substring(0, finalLen);
    }
    else
    {
      finalStr = "";
      for (int i = 0; i < finalLen - strData.length(); i++)
      {
        finalStr = finalStr + " ";
      }
      finalStr = finalStr + strData;
    }

    return (finalStr);
  } // format(BigDecimal, int, int)

  public static String format(Double doubleData, int precision, int scale)
  throws Exception
  {
    double doubData;
    String finalStr;

    doubData = doubleData.doubleValue();
    return (format(doubData, precision, scale));
  } // format(Double, int, int)
} // Data

class Db
{
  public String alias;
  public String server;
  public int    portNumber = 0; // 0 indicates legacy type 2 connection,
                                // < 0 use universal type 2 connection
  public String userId;
  public String password;
  public Connection con = null;

  public Db()
  {
  }

  public Db(String argv[]) throws Exception
  {
    if( argv.length > 5 ||
        ( argv.length == 1 &&
          ( argv[0].equals( "?" )               ||
            argv[0].equals( "-?" )              ||
            argv[0].equals( "/?" )              ||
            argv[0].equalsIgnoreCase( "-h" )    ||
            argv[0].equalsIgnoreCase( "/h" )    ||
            argv[0].equalsIgnoreCase( "-help" ) ||
            argv[0].equalsIgnoreCase( "/help" ) ) ) )
    {
      throw new Exception(
        "Usage: prog_name [dbAlias] [userId passwd] (use legacy JDBC type 2 driver)\n" +
        "       prog_name -u2 [dbAlias] [userId passwd] (use universal JDBC type 2 driver)\n" +
        "       prog_name [dbAlias] server portNum userId passwd (use universal JDBC type 4 driver)" );
    }

    switch (argv.length)
    {
      case 0:  // Type 2, use all defaults
        alias = "sample";
        userId = "";
        password = "";
        break;
      case 1:  // Type 2, dbAlias specified or Type 2 Universal
        if (argv[0].equalsIgnoreCase("-u2"))
        {
           alias ="sample";
           portNumber = -1;
        }
        else
        {
           alias = argv[0];
        }

        userId = "";
        password = "";
        break;
      case 2:  // Type 2, userId & passwd specified
        if (argv[0].equalsIgnoreCase("-u2"))
        {
           alias = argv[1];
           userId = "";
           password = "";
           portNumber = -1;
        }
        else
        {
           alias = "sample";
           userId = argv[0];
           password = argv[1];
        }

        break;
      case 3:  // Type 2, dbAlias, userId & passwd specified or Type 2 Universal
        if (argv[0].equalsIgnoreCase("-u2"))
        {
           alias = "sample";
           userId = argv[1];
           password = argv[2];
           portNumber = -1;
        }
        else
        {
           alias = argv[0];
           userId = argv[1];
           password = argv[2];
        }

        break;
      case 4:  // Type 4, use default dbAlias or Type 2 Universal
        if (argv[0].equalsIgnoreCase("-u2"))
        {
           alias = argv[1];
           userId = argv[2];
           password = argv[3];
           portNumber = -1;
        }
        else
        {
           alias = "sample";
           server = argv[0];
           portNumber = Integer.valueOf( argv[1] ).intValue();
           userId = argv[2];
           password = argv[3];
        }
        break;
      case 5:  // Type 4, everything specified
        if (! argv[0].equalsIgnoreCase("-u2"))
        {
           alias = argv[0];
           server = argv[1];
           portNumber = Integer.valueOf( argv[2] ).intValue();
           userId = argv[3];
           password = argv[4];
        }
        break;
    }
  } // Db Constructor

  public Connection connect() throws Exception
  {
    String url = null;

    // In Partitioned Database environment, set this to the node number
    // to which you wish to connect (leave as "0" in non-Partitioned Database environment)
    String nodeNumber = "0";

    Properties props = new Properties();

    if ( portNumber < 0 )
    {
      url = "jdbc:db2:" + alias;
      System.err.println(
        "Connect to '" + alias + "' database using JDBC Universal type 2 driver." );
      Class.forName("com.ibm.db2.jcc.DB2Driver").newInstance();
    }
    else if( portNumber == 0 )
    {
      url = "jdbc:db2:" + alias;
      System.err.println(
        "Connect to '" + alias + "' database using JDBC type 2 driver." );
      Class.forName("COM.ibm.db2.jdbc.app.DB2Driver").newInstance();
    }
    else
    {
      url = "jdbc:db2://" + server + ":" + portNumber + "/" + alias;
      System.err.println(
        "Connect to '" + alias + "' database using JDBC type 4 driver." );
      Class.forName("com.ibm.db2.jcc.DB2Driver").newInstance();
    }

    if( null != userId )
    {
      props.setProperty("user", userId);
      props.setProperty("password", password);
    }

    props.setProperty("CONNECTNODE", nodeNumber);

    con = DriverManager.getConnection( url, props );

    // enable transactions
    con.setAutoCommit(false);
    return con;
  } // connect

  public void disconnect() throws Exception
  {
    System.err.println();
    System.err.println("Disconnect from '" + alias + "' database.");

    // makes all changes made since the previous commit/rollback permanent
    // and releases any database locks currrently held by the Connection.
    con.commit();

    // immediately disconnects from database and releases JDBC resources
    con.close();
  } // disconnect
} // Db

class JdbcException extends Exception
{
  Connection conn;

  public JdbcException(Exception e)
  {
    super(e.getMessage());
    conn = null;
  }

  public JdbcException(Exception e, Connection con)
  {
    super(e.getMessage());
    conn = con;
  }

  public void handle()
  {
    System.err.println(getMessage());
    System.err.println();

    if (conn != null)
    {
      try
      {
        System.err.println("--Rollback the transaction-----");
        conn.rollback();
        System.err.println("  Rollback done!");
      }
      catch (Exception e)
      {
      };
    }
  } // handle

  public void handleExpectedErr()
  {
    System.err.println();
    System.err.println(
      "**************** Expected Error ******************\n");
    System.err.println(getMessage());
    System.err.println(
      "**************************************************");
  } // handleExpectedError
} // JdbcException

