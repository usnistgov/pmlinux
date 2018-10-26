package sql;

import java.sql.*;
import sql.ParamType;
import pm.PMAccess;

public class MySQL_Functions {
	public static final String GET_ALLOWED_OPERATOION_FOR_USER 	= "{? = call allowed_operations_for_user(?,?)}";



	private static ParamType paramType;
	

	public static void registerOutParamType(ParamType p){
		paramType = p;
	}

	public static Object executeFunction(String sql, ParamType outParamType, Object ... args) throws Exception{
        return executeFunction(sql, params(outParamType, args));
    }

    private static MySQL_Parameters params(ParamType outParamType, Object ... args){
        MySQL_Parameters params = new MySQL_Parameters();
        params.setOutParamType(outParamType);
        for(int i = 0; i < args.length; i++){
            Object arg = args[i];
            if(arg instanceof String){
                params.addParam(ParamType.STRING, arg);
            }else if(arg instanceof Integer){
                params.addParam(ParamType.INT, arg);
            }else if(arg instanceof Boolean){
                params.addParam(ParamType.BOOLEAN, arg);
            }
        }
        return params;
    }

	/**
	 * Method to prepare a function
	 * @param sql the sql statement to be executed
	 * @param params a 2D array of parameters and their data types (each row will be a value, type pair: [value, type]).
	 */
	public static Object executeFunction(String sql, MySQL_Parameters params)
			throws Exception {
		Statement stmt = null;
		CallableStatement cs = null;
		cs = PMAccess.conn.prepareCall(sql);
		stmt = PMAccess.conn.createStatement();
		stmt.executeUpdate("use policydb");
		stmt.executeUpdate("SET SQL_SAFE_UPDATES = 0;");
		try{
			if (params != null) {
				String type = params.getOutParamType().getName();
				if (type.equals(ParamType.INT.getName())) {
					cs.registerOutParameter(1, Types.INTEGER);
				} else if (type.equals(ParamType.STRING.getName())) {
					cs.registerOutParameter(1, Types.VARCHAR);
				} else if (type.equals(ParamType.BOOLEAN.getName())) {
					cs.registerOutParameter(1, Types.BOOLEAN);
				}

				for (int i = 0; i < params.getParams().size(); i++) {
					type = params.getParams().get(i)[0];
					String value = params.getParams().get(i)[1];
					if (type.equals(ParamType.INT.getName())) {
						if (value == null) {
							cs.setNull(i + 2, Types.INTEGER);
						} else {
							cs.setInt(i + 2, Integer.parseInt(value));
						}

					} else if (type.equals(ParamType.STRING.getName())) {
						cs.setString(i + 2, value);
					} else if (type.equals(ParamType.BOOLEAN.getName())) {
						cs.setBoolean(i + 2, Boolean.parseBoolean(value));
					}
				}
				cs.execute();
			}

			Object ret = cs.getObject(1);
			PMAccess.conn.close();
			return ret;
        }catch(Exception e){
            System.out.println("exception in function: " + e.getMessage());
            throw new Exception("Exception in Function: " + e.getMessage());
		}finally{
			PMAccess.conn.close();
		}
	}
}
