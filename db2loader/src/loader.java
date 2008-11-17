/**
 * Created by IntelliJ IDEA.
 * User: Павел
 * Date: 19.05.2008
 * Time: 18:12:45
 * To change this template use File | Settings | File Templates.
 */

import java.sql.*;
import java.util.*;
import java.util.Date;
import java.io.*;
import java.text.SimpleDateFormat;            // JDBC classes

/**
 * @noinspection ALL
 */
public class loader {
    private static String m_procName;

    /** */
    public static void main(String argv[]) {
        Connection con = null;
        PrintStream out = null;
        PrintStream log = null;

        try {
            int posProcedure = GetParamsPos(argv);

            String[] con_params = (String[]) Arrays.copyOfRange(argv, 0, posProcedure);
            Db db = new Db(con_params);

            db.connect();
            con = db.con;

            String out_file_name = ParceFileName(GetArg(argv, "-o"));
            if (!out_file_name.isEmpty()) {
                out = new PrintStream(
                        new BufferedOutputStream(
                                new FileOutputStream(out_file_name)
                        )
                );
                System.setOut(out);
            }
            String log_file_name = ParceFileName(GetArg(argv, "-l"));
            if (!log_file_name.isEmpty()) {
                log = new PrintStream(
                        new BufferedOutputStream(
                                new FileOutputStream(log_file_name)
                        )
                );
                System.setErr(log);
            }

            System.err.println(String.format("Start date: %s", Calendar.getInstance().getTime().toString()));

            StringBuffer sb = new StringBuffer();
            for (int i = 0; i < argv.length; ++i) {
                sb.append(argv[i]);
                sb.append(" ");
            }
            System.err.println(String.format("Parameters: %s", sb.toString()));

            m_procName = GetArg(argv, "-p");
            if (m_procName.isEmpty()) {
                throw new Exception("You should define procedure name");
            }

            String[] proc_params;
            proc_params = (String[]) Arrays.copyOfRange(argv, posProcedure, argv.length);

            System.err.println("Start loading...");

            String in_file_name = ParceFileName(GetArg(argv, "-w"));
            String res_table_name = GetArg(argv, "-t");
            if (in_file_name.length() > 0) {
                BufferedReader in;
                in = new BufferedReader(new FileReader(in_file_name));

                String argSkip = GetArg(argv, "-h");
                int skip = argSkip.length() == 0 ? 0 : Integer.parseInt(argSkip);
                while (in.ready() && skip > 0) {
                    String s = in.readLine();
                    skip--;
                }
                while (in.ready()) {
                    String s = in.readLine();
                    CallProcedure(con, s, proc_params);
                    if (res_table_name.length() > 0) {
                        GetResultsFromTable(con, res_table_name);
                    }
                    con.commit();
                }
                in.close();
            } else {
                CallProcedure(con, "", proc_params);
                if (res_table_name.length() > 0) {
                    GetResultsFromTable(con, res_table_name);
                }
                con.commit();
            }

            System.err.println(String.format("Loaded %d records sucessfully.", new Integer(m_loader_cnt)));
            System.err.println(String.format("Finish date: %s", Calendar.getInstance().getTime().toString()));

            if (out != null) {
                out.close();
            }
            if (log != null) {
                log.close();
            }
            con.close();
        }
        catch (Exception e) {
            try {
                con.rollback();
                con.close();
            }
            catch (Exception x) {
                x.printStackTrace();
            }
            e.printStackTrace();

            if (out != null) {
                out.close();
            }
            if (log != null) {
                log.close();
            }
        }
    }

    /** */
    private static String ParceFileName(String filename) {
        return filename.replace("%d", new SimpleDateFormat("yyyyMMdd").format(new Date()));
    }

    /** */
    private static String GetArg(String[] argv, String sParam) {
        int pos = 0;
        for (int i = 0; i < argv.length; i++) {
            String aArgv = argv[i];
            if (aArgv.startsWith(sParam)) {
                return aArgv.substring(sParam.length());
            } else {
                ++pos;
            }
        }
        return "";
    }

    /** */
    private static int GetParamsPos(String[] argv) {
        int pos = 0;
        for (int i = 0; i < argv.length; i++) {
            String aArgv = argv[i];
            if (aArgv.startsWith("-")) {
                break;
            } else {
                ++pos;
            }
        }
        return pos;
    }

    /** */
    private static boolean SkippedParam(String param) {
        return
                param.startsWith("-o") ||
                        param.startsWith("-w") ||
                        param.startsWith("-h") ||
                        param.startsWith("-t") ||
                        param.startsWith("-l") ||
                        param.startsWith("-p");
    }

    /** */
    private static void CallProcedure(Connection con, String s, String params[]) throws Exception {

        StringBuffer sb = new StringBuffer();
        sb.append("CALL ");
        sb.append(m_procName);
        sb.append("(");

        for (int i = 0; i < params.length; ++i) {
            if (SkippedParam(params[i])) {
                continue;
            }
            sb.append("?,");
        }
        if (sb.charAt(sb.length() - 1) == ',') {
            sb.setCharAt(sb.length() - 1, ')');
        } else {
            sb.append(")");
        }
        CallableStatement callStmt = con.prepareCall(sb.toString());

        try {
            Map outParams = new TreeMap();
            int param_num = 1;
            for (int i = 1; i < params.length; ++i) {
                if (SkippedParam(params[i])) {
                    continue;
                }

                if (params[i].startsWith("-i")) {
                    int nval = ParseIntParam(s, params[i].substring(2));
                    callStmt.setInt(param_num, nval);
                } else if (params[i].startsWith("-s")) {
                    String sval = ParseStringParam(params[i].substring(2));
                    callStmt.setString(param_num, sval);
                } else if (params[i].startsWith("-ri")) {
                    callStmt.registerOutParameter(param_num, Types.INTEGER);
                    outParams.put(new Integer(param_num), new Integer(Types.INTEGER));
                } else if (params[i].startsWith("-rs")) {
                    callStmt.registerOutParameter(param_num, Types.VARCHAR);
                    outParams.put(new Integer(param_num), new Integer(Types.VARCHAR));
                } else {
                    throw new Exception(String.format("Not valid parameter: '%s'", params[i]));
                }
                ++param_num;
            }

            callStmt.execute();

            ResultSet rs = callStmt.getResultSet();
            if (rs != null) {
                try {
                    GetResultSet(rs);
                    rs.close();
                }
                catch (Exception e) {
                    rs.close();
                    throw e;
                }
            }

            Iterator it = outParams.entrySet().iterator();
            if (it.hasNext()) {
                System.err.print("Results: ");
                while (it.hasNext()) {
                    Map.Entry<Integer, Integer> entry = (Map.Entry<Integer, Integer>) it.next();
                    if (entry.getValue() == Types.INTEGER) {
                        System.err.print(String.format("%d ", callStmt.getInt(entry.getKey())));
                    } else if (entry.getValue() == Types.VARCHAR) {
                        System.err.print(String.format("%s ", callStmt.getString(entry.getKey())));
                    }
                }
                System.err.println("");
            }

            callStmt.close();
        }
        catch (Exception x) {
            callStmt.close();
            throw x;
        }
    }

    /** */
    private static int ParseIntParam(String s, String param) {
        if (param.startsWith("%p")) {
            int pos = Integer.parseInt(param.substring(2));
            int sep_cnt = 0;
            int len = s.length();
            int prev_sep_pos = 0;
            for (int i = 0; i < len; ++i) {
                if (s.charAt(i) == ';') {
                    ++sep_cnt;
                    if (sep_cnt == pos) {
                        return Integer.parseInt(s.substring(prev_sep_pos, i));
                    } else {
                        prev_sep_pos = i + 1;
                    }
                }
            }
        }
        return Integer.parseInt(param);
    }

    /** */
    private static int ProcessDateInStringParam(String param, int i, Calendar now) {
        int len = param.length();
        if (i + 1 < len) {
            ++i;
            if (param.charAt(i) == '[') {
                if (i + 1 < len) {
                    ++i;
                    StringBuffer sDelta = new StringBuffer();
                    for (; i < len; ++i) {
                        if (param.charAt(i) == 'y') {
                            Integer y = new Integer(sDelta.toString());
                            now.set(Calendar.YEAR, now.get(Calendar.YEAR) - y);
                        } else if (param.charAt(i) == 'm') {
                            Integer m = new Integer(sDelta.toString());
                            now.set(Calendar.MONTH, now.get(Calendar.MONTH) - m);
                        } else if (param.charAt(i) == 'd') {
                            Integer d = new Integer(sDelta.toString());
                            now.set(Calendar.DAY_OF_MONTH, now.get(Calendar.DAY_OF_MONTH) - d);
                        } else if (param.charAt(i) == ']') {
                            break;
                        } else {
                            sDelta.append(param.charAt(i));
                        }
                    }
                }
            }
        }
        return i;
    }

    /** */
    private static String ParseStringParam(String param) {
        Calendar now = Calendar.getInstance();
        StringBuffer sb = new StringBuffer();
        int len = param.length();
        for (int i = 0; i < len; ++i) {
            boolean bProcessed = false;
            if (param.charAt(i) == '%') {
                if (i + 1 < len) {
                    if (param.charAt(i + 1) == 'd') {
                        i = ProcessDateInStringParam(param, i + 1, now);
                        sb.append(new SimpleDateFormat("yyyy-MM-dd").format(now.getTime()));
                        bProcessed = true;
                    } else if (param.charAt(i + 1) == 'y') {
                        ++i;
                        sb.append(new SimpleDateFormat("yyyy").format(now.getTime()));
                        bProcessed = true;
                    }
                }
            }
            if (bProcessed == false) {
                sb.append(param.charAt(i));
            }
        }
        return sb.toString();
    }

    /** */
    private static void GetResultsFromTable(Connection con, String res_table_name) throws Exception {
        Statement stmt = con.createStatement();
        try {
            ResultSet rs = stmt.executeQuery("select * from " + res_table_name);
            if (rs != null) {
                try {
                    GetResultSet(rs);
                    rs.close();
                }
                catch (Exception e) {
                    rs.close();
                    throw e;
                }
            }
            stmt.close();
        }
        catch (Exception e) {
            stmt.close();
            throw e;
        }
    }

    /** */
    private static boolean m_header_printed = false;

    /** */
    private static int m_loader_cnt = 0;

    /** */
    private static void GetResultSet(ResultSet rs) throws SQLException {

        ResultSetMetaData stmtInfo = rs.getMetaData();

        int numOfColumns = stmtInfo.getColumnCount();

        // print columns labels
        if (!m_header_printed) {
            for (int i = 1; i <= numOfColumns; i++) {
                System.out.print(stmtInfo.getColumnLabel(i));
                if (i == numOfColumns) {
                    System.out.println("");
                } else {
                    System.out.print(";");
                }
            }
            m_header_printed = true;
        }

        // print data rows
        while (rs.next()) {
            m_loader_cnt++;
            for (int i = 1; i <= numOfColumns; i++) {
                String sval = rs.getString(i);
                System.out.print(sval == null ? "" : sval);
                if (i == numOfColumns) {
                    System.out.println("");
                } else {
                    System.out.print(";");
                }
            }
            if (m_loader_cnt % 10000 == 0) {
                System.err.println(String.format("Loaded %d records...", new Integer(m_loader_cnt)));
            }
        }
    }

}
