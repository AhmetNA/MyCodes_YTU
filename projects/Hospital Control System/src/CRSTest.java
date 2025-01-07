import org.junit.Test;

import java.text.ParseException;
import java.util.Date;

import static org.junit.jupiter.api.Assertions.*;

public class CRSTest {

    @Test
    public void makeRendezvous() throws DuplicateInfoException {
        CRS crs = new CRS();
        crs.addHospital(new Hospital(1, "Hospital"));
        crs.getHospital(1).addSection("Heart Surgery");
        crs.getHospital(1).addSection("Brain Surgery");

        crs.addPatient(new Patient("Patient1" , 122));
        crs.addPatient(new Patient("Patient2" , 123));

        crs.getHospital(1).getSection(1).addDoctor(new Doctor("Doctor1", 1, 1, crs.getHospital(1).getSection(1)));
        crs.getHospital(1).getSection(2).addDoctor(new Doctor("Doctor2", 2, 2, crs.getHospital(1).getSection(2)));

        Date date1 = new Date(2021, 12, 15, 15, 0);
        Date date2 = new Date(2021, 12, 15, 16, 0);

        try {
            crs.makeRendezvous(122, 1, 1, 1, date1);
            crs.makeRendezvous(123, 1, 2, 2, date2);
        } catch (IDException e) {
            e.printStackTrace();
        } catch (ParseException e) {
            throw new RuntimeException(e);
        }

        assertEquals(1, crs.getHospital(1).getSection(1).getRendezvousList().size());
        assertEquals(1, crs.getHospital(1).getSection(2).getRendezvousList().size());
    }

    @Test
    public void listAllRendezvous() throws DuplicateInfoException {
        CRS crs = new CRS();
        crs.addHospital(new Hospital(1, "Hospital"));
        crs.getHospital(1).addSection("Heart Surgery");
        crs.getHospital(1).addSection("Brain Surgery");

        crs.addPatient(new Patient("Patient1" , 122));
        crs.addPatient(new Patient("Patient2" , 123));

        crs.getHospital(1).getSection(1).addDoctor(new Doctor("Doctor1", 1, 1, crs.getHospital(1).getSection(1)));
        crs.getHospital(1).getSection(2).addDoctor(new Doctor("Doctor2", 2, 2, crs.getHospital(1).getSection(2)));

        Date date1 = new Date(2021, 12, 15, 15, 0);
        Date date2 = new Date(2021, 12, 15, 16, 0);

        try {
            crs.makeRendezvous(122, 1, 1, 1, date1);
            crs.makeRendezvous(123, 1, 2, 2, date2);
        } catch (IDException | ParseException e) {
            e.printStackTrace();
        }

        assertEquals(2, crs.getHospital(1).getAllRendezvous().size());
    }

    @Test
    public void listDoctors() throws DuplicateInfoException {
        Section section = new Section(1, "Heart Surgery");
        section.addDoctor(new Doctor("Doctor1", 1, 1, section));
        section.addDoctor(new Doctor("Doctor2", 2, 2, section));

        assertEquals(2, section.getDoctors().size());
    }

}