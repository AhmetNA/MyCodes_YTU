import java.text.ParseException;
import java.util.*;
import java.io.*;

public class CRS {
    private HashMap<Long, Patient> patients;
    private LinkedList<Rendezvous> rendezvous;
    private HashMap<Integer, Hospital> hospitals;

    public CRS() {
        patients = new HashMap<>();
        rendezvous = new LinkedList<>();
        hospitals = new HashMap<>();
    }

    public synchronized Hospital getHospital(int id) {return hospitals.get(id);}

    public synchronized HashMap<Long, Patient> getPatients() {return patients;}

    public synchronized void addRendezvous(Rendezvous rendezvous) {this.rendezvous.add(rendezvous);}

    public synchronized void addHospital(Hospital hospital) throws DuplicateInfoException {
        if(hospitals.containsKey(hospital.getId())) {
            throw new DuplicateInfoException("Hospital already exists");
        }
        hospitals.put(hospital.getId(), hospital);
    }

    public synchronized void addPatient(Patient patient) throws DuplicateInfoException {
        if(patients.containsKey(patient.getNational_Id())) {
            throw new DuplicateInfoException("This national ID already exists");
        }
        for (Hospital hospital : hospitals.values()) {
            for (Section section : hospital.getAllSections()) {
                for (Doctor doctor : section.getDoctors()) {
                    if (doctor.getNational_Id() == patient.getNational_Id()) {
                        throw new DuplicateInfoException("This national ID already exists");
                    }
                }
            }
        }
        patients.put(patient.getNational_Id(), patient);}

    public void makeRendezvous(long patientID, int hospitalID, int sectionID, int diplomaID, Date desiredDate) throws IDException, FullScheduleException, ParseException {
        Patient patient = patients.get(patientID);
        if (patient == null) {
            throw new IDException("Patient not found");
        }

        // Hospital finder
        for (Hospital hospital : hospitals.values()) {
            if (hospital.getId() == hospitalID) {
                Section section = hospital.getSection(sectionID);
                if (section == null) {
                    throw new IDException("Section not found");
                }

                Doctor doctor = section.getDoctor(diplomaID);
                if (doctor == null) {
                    throw new IDException("Doctor not found");
                }

                if (doctor.isAvailable(desiredDate)) {
                    doctor.getSchedule().addRendezvous(patient, desiredDate, doctor);
                    this.rendezvous.add(new Rendezvous(patient, desiredDate, doctor, section));
                    return;
                }
                else {
                    patients.remove(patientID);
                    System.out.println("Doctor is not available at this time");
                    throw new FullScheduleException("Doctor is not available at this time");
                }
            }
        }
    }

    public synchronized void saveTablesToDisk(String fullPath) {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(fullPath + "patients.dat"))) {
            oos.writeObject(patients);
            System.out.println("Patients saved");
        } catch (Exception e) {
            System.out.println("Patients couldn't saved");
            e.printStackTrace();
        }

        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(fullPath + "rendezvous.dat"))) {
            oos.writeObject(rendezvous);
            System.out.println("Rendezvous saved");
        } catch (Exception d) {
            System.out.println("Rendezvous couldn't saved");
            d.printStackTrace();
        }

        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(fullPath + "hospitals.dat"))) {
            oos.writeObject(hospitals);
            System.out.println("Hospitals saved");
        } catch (Exception c) {
            System.out.println("Hospitals couldn't saved");
            c.printStackTrace();
        }

    }

    public synchronized void loadTablesFromDisk (String fullPath){
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(fullPath + "patients.dat"))) {
            patients = (HashMap<Long, Patient>) ois.readObject();
            System.out.println("Patients loaded");
        } catch (Exception e) {
            System.out.println("Patients couldn't loaded");
            e.printStackTrace();
        }

        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(fullPath + "rendezvous.dat"))) {
            rendezvous = (LinkedList<Rendezvous>) ois.readObject();
            System.out.println("Rendezvous loaded");
        } catch (Exception e) {
            System.out.println("Rendezvous couldn't loaded");
            e.printStackTrace();
        }

        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(fullPath + "hospitals.dat"))) {
            hospitals = (HashMap<Integer, Hospital>) ois.readObject();
            System.out.println("Hospitals loaded");
        } catch (Exception e) {
            System.out.println("Hospitals couldn't loaded");
            e.printStackTrace();
        }
    }
}

