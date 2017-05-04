package Database;

import Utility.Facility;
import Utility.Rate;
import Utility.Room;
import com.sun.org.apache.regexp.internal.RE;
import org.omg.CORBA.PUBLIC_MEMBER;

import java.sql.*;
import java.sql.Date;
import java.util.*;

/**
 * Created by partha on 08-Dec-16.
 */
public class DBconnection {


    private String DBurl = "jdbc:oracle:thin:@localhost:1521:ORCL";
    private String DBusername = "project";
    private String DBpassword = "304";
    private Connection conn = null;
    private  boolean connectionStatus=false;
    public  static  final int SQL_ERROR=-1111;

    public DBconnection() {


        try {
            Class.forName("oracle.jdbc.OracleDriver");
            conn = DriverManager.getConnection(DBurl, DBusername, DBpassword);
            if(conn!=null)
            {
                connectionStatus=true;
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();


        }
        catch (SQLException e)
        {
            e.printStackTrace();

        }

    }

    public boolean isConnectionStatus() {
        return connectionStatus;
    }
    public  void closeConnection()
    {
        if(conn!=null)
        {
            try {
                conn.close();
                connectionStatus = false;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }


    }
    public boolean insertEmployee(String first_name,String last_name,String department,String  designation,String contact,String email,String password)
    {

        PreparedStatement statement= null;
        try {
            statement = conn.prepareStatement("insert into EMPLOYEE values(EMPLOYEE_ID_SEQ.NEXTVAL,?,?,?,?,?,?,?)");
            statement.setString(1,email);
            statement.setString(2,contact);
            statement.setString(3,first_name);
            statement.setString(4,last_name);
            statement.setString(5,department);
            statement.setString(6,designation);
            statement.setString(7,password);
            int state=statement.executeUpdate();
            if(state==1)
            {
                return  true;
            }

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return false;

    }
    public ArrayList<Facility> getFacility(String facility, String  date)
    {
        PreparedStatement statement=null;
        ArrayList<Facility> arrayList=new ArrayList<>();

            String query="SELECT FACILITY_ID,PRICE,SPECIALITY FROM FACILITY WHERE FACILITY_TYPE = '"+facility+
                    "'AND FACILITY_ID NOT IN (SELECT FACILITY_ID FROM FACILITY_BOOKING WHERE BOOKING_ID IN(SELECT  BOOKING_ID FROM BOOKING WHERE DATE_FROM =TO_DATE('"+date+"','YYYY-MM-DD')))";

        try {
            statement=conn.prepareStatement(query);
            System.out.println("it is");
            ResultSet rs=statement.executeQuery();
            for(int i=0;rs.next();i++)
            {
                arrayList.add(new Facility(rs.getString("FACILITY_ID"),facility,rs.getString("SPECIALITY"),rs.getDouble("PRICE")));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return  arrayList;

    }
    public boolean isPossible(String df,int [] facility)
    {
        PreparedStatement statement=null;
        ArrayList<Integer> result=new ArrayList<>();
        String sql="SELECT FACILITY_ID FROM FACILITY WHERE "+
                " FACILITY_ID NOT IN (SELECT FACILITY_ID FROM FACILITY_BOOKING WHERE BOOKING_ID IN(SELECT  BOOKING_ID FROM BOOKING WHERE DATE_FROM =TO_DATE('"+df+"','YYYY-MM-DD')))";

        try {
            statement=conn.prepareStatement(sql);
            ResultSet rs=statement.executeQuery();

            for(int i=0;rs.next();i++)
            {
                result.add(rs.getInt("FACILITY_ID"));
            }
            for(int i=0;i<facility.length;i++)
            {
                if(!(result.contains(facility[i])))
                {
                    return false;
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return  true;
    }
    public boolean isPossible(String df,String dt,int[] rooms)
    {

        PreparedStatement statement=null;
        ArrayList<Integer> query=new ArrayList<>();
        String sql="SELECT ROOM_NO FROM ROOM WHERE " +
                " ROOM_NO NOT IN (SELECT  ROOM_NO FROM ROOM_BOOKING WHERE BOOKING_ID IN(SELECT BOOKING_ID FROM BOOKING WHERE(DATE_FROM = TO_DATE ('"+df+"' , 'YYYY-MM-DD')AND DATE_TO = TO_DATE ('"+dt+"' , 'YYYY-MM-DD'))" +
                "OR(DATE_FROM BETWEEN TO_DATE ('"+df+"' , 'YYYY-MM-DD')AND TO_DATE ('"+dt+"' , 'YYYY-MM-DD'))OR (DATE_TO BETWEEN TO_DATE ('"+df+"' , 'YYYY-MM-DD')" +
                "AND TO_DATE ('"+dt+"' , 'YYYY-MM-DD'))))";
        try {
            statement=conn.prepareStatement(sql);
            ResultSet rs=statement.executeQuery();

            for(int i=0;rs.next();i++)
            {
                query.add(rs.getInt("ROOM_NO"));
            }
            for(int i=0;i<rooms.length;i++)
            {
                if(!(query.contains(rooms[i])))
                {
                    return false;
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return  true;


    }
    public ArrayList<Room> getRoomList(String sp, String  cap, String ac, String wifi, String pf, String pt, String  df, String  dt)
    {
        ArrayList<Room> room=new ArrayList<>();
        PreparedStatement statement=null;




        try {

            System.out.println("<h1>"+sp+cap+ac+wifi+pf+pt+df+dt+"</h1>");
            String  query="SELECT ROOM_NO,SPECIALITY,ROOM_COST FROM ROOM WHERE CAPACITY ='"+cap+"' AND SPECIALITY ='"+sp+"'AND AIR_CONDITIONER ='"+ac+"' AND WI_FI = '"+wifi+"'  AND (" +
                    "ROOM_COST BETWEEN "+pf +
                    " AND "+pt +
                    ")" +
                    "AND ROOM_NO NOT IN (SELECT  ROOM_NO FROM ROOM_BOOKING WHERE BOOKING_ID IN(SELECT BOOKING_ID FROM BOOKING WHERE(DATE_FROM = TO_DATE ('"+df+"' , 'YYYY-MM-DD')AND DATE_TO = TO_DATE ('"+dt+"' , 'YYYY-MM-DD'))" +
                    "OR(DATE_FROM BETWEEN TO_DATE ('"+df+"' , 'YYYY-MM-DD')AND TO_DATE ('"+dt+"' , 'YYYY-MM-DD'))OR (DATE_TO BETWEEN TO_DATE ('"+df+"' , 'YYYY-MM-DD')" +
                    "AND TO_DATE ('"+dt+"' , 'YYYY-MM-DD'))))";
            statement=conn.prepareStatement(query);


            System.out.println("it is");
            ResultSet rs=statement.executeQuery();

            for(int i=0;rs.next();i++)
            {
                room.add(new Room(rs.getString("SPECIALITY"),rs.getDouble("ROOM_COST"),rs.getInt("ROOM_NO")));
            }
            room.add(new Room(rs.getString("SPECIALITY"),rs.getDouble("ROOM_COST"),rs.getInt("ROOM_NO")));


        } catch (SQLException e) {
            e.printStackTrace();
        }
        return room;


    }
    public  int bookFacility(String df,int gid)
    {
        CallableStatement statement=null;

        int bid=0;

        try {

            String sql = "{ ? = call INSERT_BOOKING_FACILITY(?,?) }";
            statement = conn.prepareCall(sql);
            statement.setString(2,df);
            statement.setInt(3,gid);
            statement.registerOutParameter(1, java.sql.Types.INTEGER);
            statement.executeUpdate();
            bid=statement.getInt(1);
            return  bid;

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return  SQL_ERROR;

    }
    public  boolean updateFacilityBook(int []facility,long facility_booking_id)
    {

        PreparedStatement statement=null;
        /*String query="UPDATE FACILITY SET  FACILITY_BOOKING_ID = "+facility_booking_id+",PAY_STATE='PENDING' WHERE FACILITY_ID= "+facility_id;
        try {
            statement=conn.prepareStatement(query);
            int state=statement.executeUpdate();
            if(state==1)
            {
                return true;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return false;*/
        /*String query="UPDATE FACILITY SET  PAY_STATE='PENDING' WHERE FACILITY_ID IN ( ";
        int i=0;
        for( i=0;i<facility.length;i++)
        {
            query+=facility[i]+",";
        }
        if(i!=0)
        {
            query = query.substring(0, query.length() - 1);
        }
        query+=")";*/

        try {
           /* statement=conn.prepareStatement(query);
            int state=statement.executeUpdate();*/

            for (int j = 0; j < facility.length; j++) {
                String sql = "INSERT INTO FACILITY_BOOKING (FACILITY_BOOKING_ID,FACILITY_ID,BOOKING_ID,PAY_STATE) VALUES (FACILITY_BOOKING_ID_SEQ.NEXTVAL," + facility[j] + "," + facility_booking_id + ",'PENDING')";
                statement = conn.prepareStatement(sql);
                statement.executeUpdate();


            }
            return  true;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return false;

    }
    public int bookRoom(String df,String  dt,int gid)
    {
        CallableStatement statement=null;

        int bid=0;

        try {

            String sql = "{ ? = call INSERT_BOOKING(?,?,?) }";
            statement = conn.prepareCall(sql);
            statement.setString(2,df);
            statement.setString(3,dt);
            statement.setInt(4,gid);
            statement.registerOutParameter(1, java.sql.Types.INTEGER);
            statement.executeUpdate();
            bid=statement.getInt(1);
            return  bid;

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return  SQL_ERROR;

    }
    public  boolean updateRoomBook(int bid,int [] room)
    {
        PreparedStatement statement=null;
        String query="UPDATE ROOM SET STATUS= 'PROCESSING'  WHERE ROOM_NO IN ( ";
        int i=0;
        for( i=0;i<room.length;i++)
        {
            query+=room[i]+",";
        }
        if(i!=0)
        {
            query = query.substring(0, query.length() - 1);
        }
        query+=")";

        try {
            statement=conn.prepareStatement(query);
            int state=statement.executeUpdate();

                for (int j = 0; j < room.length; j++) {
                    String sql = "INSERT INTO ROOM_BOOKING (ROOM_BOOKING_ID,ROOM_NO,BOOKING_ID,PAY_STATE) VALUES (ROOM_BOOKING_ID_SEQ.NEXTVAL," + room[j] + "," + bid + ",'PENDING')";
                    statement = conn.prepareStatement(sql);
                    statement.executeUpdate();


            }
            return  true;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return false;
    }
    public int insertGuest(String first_name,String last_name,String address,String  contact,String nid,String passport,String person)
    {

        CallableStatement statement= null;
        int gid=0;

        System.out.println("i am here");
        try {

            String sql = "{ ? = call INSERT_GUEST(?,?,?,?,?,?,?) }";
             statement = conn.prepareCall(sql);
            statement.setString(2,first_name);
            statement.setString(3,last_name);
            statement.setString(4,address);
            statement.setString(5,contact);
            statement.setInt(6, Integer.parseInt(person));
            statement.setString(7,passport);
            statement.setString(8,nid);
            statement.registerOutParameter(1, java.sql.Types.INTEGER);
            statement.executeUpdate();
            gid=statement.getInt(1);



            //System.out.println("It is state "+state);

            return gid;

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return SQL_ERROR;

    }
    public int insertGuest(String first_name,String last_name,String address,String  contact,String nid,String passport,String person,String memberno)
    {

        CallableStatement statement= null;
        int gid=0;

        System.out.println("i am here");
        try {

            String sql = "{ ? = call INSERT_GUEST_MEMBER(?,?,?,?,?,?,?) }";
            statement = conn.prepareCall(sql);
            statement.setString(2,first_name);
            statement.setString(3,last_name);
            statement.setString(4,address);
            statement.setString(5,contact);
            statement.setString(6,passport);
            statement.setString(7,nid);
            statement.setInt(8, Integer.parseInt(memberno));
            statement.registerOutParameter(1, java.sql.Types.INTEGER);
            statement.executeUpdate();
            gid=statement.getInt(1);



            //System.out.println("It is state "+state);

            return gid;

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return SQL_ERROR;

    }
    public String getDesignation(String  username,String  password)
    {
        PreparedStatement statement=null;
        String designation= String.valueOf(SQL_ERROR);


        String sql="SELECT DESIGNATION FROM EMPLOYEE WHERE EMPLOYEE_ID="+username+" AND PASSWORD = "+password;
        try {
            statement=conn.prepareStatement(sql);
            ResultSet rs=statement.executeQuery();

            if(rs.next())
            {
                designation=rs.getString("DESIGNATION");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return  designation;
    }
    public String getName(String  username,String  password)
    {
        PreparedStatement statement=null;
        String name= String.valueOf(SQL_ERROR);


        String sql="SELECT EMPLOYEE_FIRST_NAME,EMPLOYEE_LAST_NAME FROM EMPLOYEE WHERE EMPLOYEE_ID="+username+" AND PASSWORD = "+password;
        try {
            statement=conn.prepareStatement(sql);
            ResultSet rs=statement.executeQuery();
            if(rs.next())
            {
                name=rs.getString("EMPLOYEE_FIRST_NAME")+"    "+rs.getString("EMPLOYEE_LAST_NAME");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return  name;
    }
    public  ArrayList<Facility>  getAllFacility(int guestId)
    {
        PreparedStatement statement=null;
        ArrayList<Facility> arrayList=new ArrayList<>();

        String sql="SELECT  FACILITY_ID,PRICE,FACILITY_TYPE,SPECIALITY FROM FACILITY WHERE FACILITY_ID IN " +
                "(SELECT FACILITY_ID FROM FACILITY_BOOKING WHERE PAY_STATE='PENDING' AND BOOKING_ID IN(SELECT BOOKING_ID" +
                " FROM BOOKING WHERE DATE_TO IS NULL AND BOOKED_BY_GUEST="+guestId+"))";

        try {
            statement=conn.prepareStatement(sql);
            ResultSet rs=statement.executeQuery();
            for(;rs.next();)
            {
                Facility facility=new Facility(rs.getString("FACILITY_ID"),rs.getString("FACILITY_TYPE"),rs.getString("SPECIALITY"),rs.getDouble("PRICE"));
                arrayList.add(facility);
            }

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return  arrayList;
    }
    public  ArrayList<Room>  getAllRoom(int guestId)
    {
        PreparedStatement statement=null;
        ArrayList<Room> arrayList=new ArrayList<>();

        String sql="SELECT  ROOM_NO,ROOM_COST,SPECIALITY FROM ROOM WHERE ROOM_NO IN " +
                "(SELECT ROOM_NO FROM ROOM_BOOKING WHERE PAY_STATE='PENDING' AND BOOKING_ID IN(SELECT BOOKING_ID" +
                " FROM BOOKING WHERE DATE_TO IS NOT NULL AND DATE_FROM IS NOT NULL AND BOOKED_BY_GUEST="+guestId+"))";

        try {
            statement=conn.prepareStatement(sql);
            ResultSet rs=statement.executeQuery();
            for(;rs.next();)
            {
                Room room=new Room(rs.getString("SPECIALITY"),rs.getDouble("ROOM_COST"),rs.getInt("ROOM_NO"));
                arrayList.add(room);
            }

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return  arrayList;
    }
    public  String memberType(int guestId)
    {
        String type=null;
        PreparedStatement statement=null;
         String sql="SELECT MEMBER_TYPE FROM CLUB_MEMBER_DATA WHERE MEMBER_ID =" +
                 "(SELECT MEMBER_ID FROM CLUB_MEMBER WHERE MEMBER_GUEST_ID="+guestId+")";

        try {
            statement=conn.prepareStatement(sql);
            ResultSet rs=statement.executeQuery();
            rs.next();
            type=rs.getString("MEMBER_TYPE");

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return  type;

    }
    public  boolean insertBill(String  refundable,String guest_id,String employee_id,String amount,String payment_method)
    {
        PreparedStatement statement=null;
        String sql="INSERT INTO BILL(BILL_ID,REFUNDABLE,BILL_DATE,AMOUNT,EMPLOYEE_ID," +
                "GUEST_ID,PAYMENT_METHOD)VALUES (BILL_ID_SEQ.NEXTVAL,'"+refundable+"',SYSDATE,"
                +amount+","+employee_id+","+guest_id+","+"'"+payment_method+"')";
        try {
            statement=conn.prepareStatement(sql);
            int state=statement.executeUpdate();
            if(state!=0)
            {
                return true;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return false;

    }
    public boolean updatePayState(String guest_id)
    {
        ArrayList<Facility>facilities=getAllFacility(Integer.parseInt(guest_id));
        ArrayList<Room> rooms=getAllRoom(Integer.parseInt(guest_id));
        String sql="UPDATE ROOM_BOOKING SET PAY_STATE='PAID' WHERE BOOKING_ID=(SELECT  BOOKING_ID" +
                " FROM BOOKING WHERE DATE_FROM IS NOT NULL AND DATE_TO IS NOT NULL AND BOOKED_BY_GUEST = "+guest_id+") AND ROOM_NO IN(";
        PreparedStatement statement=null;
        int k=0;
        for(int i=0;i<rooms.size();i++)
        {
            sql+=rooms.get(i).getRoom_no()+",";
            k=1;
        }
        if(k!=0)
        {
            sql = sql.substring(0, sql.length() - 1);
        }
        sql+=")";
        try {
            statement=conn.prepareStatement(sql);
            statement.executeUpdate();
            sql="UPDATE FACILITY_BOOKING SET PAY_STATE='PAID' WHERE  BOOKING_ID=(SELECT  BOOKING_ID" +
                    " FROM BOOKING WHERE DATE_FROM IS NOT NULL AND DATE_TO IS NULL AND BOOKED_BY_GUEST = "+guest_id+") AND FACILITY_ID IN(";
            k=0;
            for(int i=0;i<facilities.size();i++)
            {
                sql+=facilities.get(i).getFacility_id()+",";
                k=1;
            }
            if(k!=0)
            {
                sql = sql.substring(0, sql.length() - 1);
            }
            sql+=")";
            statement=conn.prepareStatement(sql);
            statement.executeUpdate();

            return  true;

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return  false;

    }
    public int changeRoomStatus(String  guestId)
    {
        ArrayList<Integer> rooms=new ArrayList<>();
        PreparedStatement statement1=null;
        String sql1="SELECT  ROOM_NO FROM ROOM WHERE ROOM_NO IN " +
                "(SELECT ROOM_NO FROM ROOM_BOOKING WHERE BOOKING_ID IN(SELECT BOOKING_ID" +
                " FROM BOOKING WHERE DATE_TO IS NOT NULL AND DATE_FROM IS NOT NULL AND BOOKED_BY_GUEST="+guestId+"))";

        try {
            statement1 = conn.prepareStatement(sql1);
            ResultSet rs = statement1.executeQuery();
            for (; rs.next(); ) {

                rooms.add(rs.getInt("ROOM_NO"));
            }
        }catch (SQLException e)
        {
            return  0;
        }
        int update=0;
        if(rooms.size()!=0)
        {
            PreparedStatement statement=null;

            String sql="UPDATE ROOM SET STATUS='OCCUPIED' WHERE ROOM_NO IN (";
            int i=0;
            for (i=0;i<rooms.size();i++)
            {
                sql+=rooms.get(i)+",";
            }
            if(i!=0)
            {
                sql=sql.substring(0,sql.length()-1);
            }
            sql+=")";
            try {
                statement=conn.prepareStatement(sql);
                update=statement.executeUpdate();
            } catch (SQLException e) {
                e.printStackTrace();
            }


        }
        return  update;
    }
    public ArrayList<Integer> getRoom(String  guestId)
    {

        ArrayList<Integer> rooms=new ArrayList<>();
        PreparedStatement statement1=null;
        String sql1="SELECT  ROOM_NO FROM ROOM WHERE ROOM_NO IN " +
                "(SELECT ROOM_NO FROM ROOM_BOOKING WHERE BOOKING_ID IN(SELECT BOOKING_ID" +
                " FROM BOOKING WHERE DATE_TO IS NOT NULL AND DATE_FROM IS NOT NULL AND BOOKED_BY_GUEST="+guestId+"))";

        try {
            statement1 = conn.prepareStatement(sql1);
            ResultSet rs = statement1.executeQuery();
            for (; rs.next(); ) {

                rooms.add(rs.getInt("ROOM_NO"));
            }
        }catch (SQLException e)
        {

        }
        return rooms;

    }
    public int getBillStatus(String  guestId)
    {
        int payment=0;
        PreparedStatement statement=null;
        String sql= "SELECT ROOM_NO FROM ROOM_BOOKING WHERE PAY_STATE='PENDING' AND BOOKING_ID IN(SELECT BOOKING_ID" +
                " FROM BOOKING WHERE DATE_TO IS NOT NULL AND DATE_FROM IS NOT NULL AND BOOKED_BY_GUEST="+guestId+")";

        try {
            statement=conn.prepareStatement(sql);
            ResultSet rs=statement.executeQuery();
            for(;rs.next();)
            {
                if(rs.getString("ROOM_NO")!=null)
                {
                    payment++;

                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
            return SQL_ERROR;
        }
        return payment;
    }
    public boolean setRoomStatus(String guest_id)
    {

        PreparedStatement statement=null;
        ArrayList<Integer> arrayList=new ArrayList<>();

        String sql="SELECT ROOM_NO FROM ROOM_BOOKING WHERE  BOOKING_ID IN(SELECT BOOKING_ID" +
                " FROM BOOKING WHERE DATE_TO IS NOT NULL AND DATE_FROM IS NOT NULL AND BOOKED_BY_GUEST="+guest_id+")";

        try {
            statement=conn.prepareStatement(sql);
            ResultSet rs=statement.executeQuery();
            for(;rs.next();)
            {

                arrayList.add(rs.getInt("ROOM_NO"));
            }

        } catch (SQLException e) {
            e.printStackTrace();
            return  false;
        }
        ArrayList<Integer> all=new ArrayList<>();
        sql="SELECT ROOM_NO FROM ROOM_BOOKING WHERE BOOKING_ID IN" +
                "(SELECT BOOKING_ID FROM BOOKING WHERE DATE_FROM IS NOT NULL AND DATE_TO IS NOT NULL AND DATE_FROM=SYSDATE)";

        try {
            statement=conn.prepareStatement(sql);
            ResultSet rs=statement.executeQuery();
            for(;rs.next();)
            {

                all.add(rs.getInt("ROOM_NO"));
            }
        } catch (SQLException e) {
            e.printStackTrace();
            return  false;
        }
        for(int i=0;i<arrayList.size();i++)
        {
            if(all.contains(arrayList.get(i)))
            {
                sql="UPDATE ROOM SET STATUS = 'PROCESSING' WHERE ROOM_NO = "+arrayList.get(i);
                try {
                    statement=conn.prepareStatement(sql);
                    statement.executeUpdate();
                } catch (SQLException e) {
                    e.printStackTrace();
                    return  false;
                }

            }
            else
            {
                sql="UPDATE ROOM SET STATUS = 'VACCANT' WHERE ROOM_NO = "+arrayList.get(i);
                try {
                    statement=conn.prepareStatement(sql);
                    statement.executeUpdate();
                } catch (SQLException e) {
                    e.printStackTrace();
                    return  false;
                }
            }
        }
        return  true;

    }
    public  int changePassword(String  eid,String op,String np)
    {
        PreparedStatement statement=null;
        int update=0;
        String  sql="UPDATE EMPLOYEE SET PASSWORD = "+np+" WHERE PASSWORD = "+op+" AND EMPLOYEE_ID = "+eid;
        try {
            statement=conn.prepareStatement(sql);
            update=statement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return update;

    }
    public  ArrayList<Rate> getdiscountPolicy(String  memberType)
    {
        PreparedStatement statement=null;
        ArrayList<Rate> data=new ArrayList<>();
        String sql="SELECT * FROM DISCOUNT WHERE DISCOUNT_MEMBER_TYPE = '"+memberType+"'";
        try {
            statement=conn.prepareStatement(sql);
            ResultSet rs=statement.executeQuery();
            for(;rs.next();)
            {
                data.add(new Rate(rs.getString("DISCOUNT_FACILITY_TYPE"),rs.getDouble("DISCOUNT_RATE")));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return  data;

    }


}