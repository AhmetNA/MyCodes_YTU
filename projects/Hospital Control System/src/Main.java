import javax.swing.*;
import java.awt.*;
import java.text.*;
import java.util.*;
import java.util.List;
import java.util.concurrent.*;

public class Main {
    private static JLabel infoLabel;
    private static final CRS crs = new CRS();

    public static void main(String[] args) throws DuplicateInfoException {
        // Create a hospital
        Hospital hospital = new Hospital(1, "Hospital");
        crs.addHospital(hospital);

        // If there is saved data, load it
        try {
            crs.loadTablesFromDisk("tables.ser");
        } catch (Exception e) {
            System.out.println("No saved data found. Starting with default values.");
        }

        // Ask for mode CLI OR GUI
        try {
            String mode = JOptionPane.showInputDialog("Enter mode (GUI/CLI/BOTH):");
            if (mode == null) return;
            mode = mode.toUpperCase();
            if (mode.equals("GUI") || mode.equals("GUİ")) {
                SwingUtilities.invokeLater(new GUIRunnable());
            } else if (mode.equals("CLI") || mode.equals("CLİ")) {
                new Thread(new CLIRunnable()).start();
            } else if (mode.equals("BOTH")) {
                SwingUtilities.invokeLater(new GUIRunnable());
                new Thread(new CLIRunnable()).start();

            } else {
                System.out.println("Invalid mode");
            }
        } catch (Exception e) {
            System.out.println("Invalid mode");
        }
    }

    public static class GUIRunnable implements Runnable {
        @Override
        public void run() {
            GUI();
        }
    }

    public static class CLIRunnable implements Runnable {
        @Override
        public void run() {
            CLI();
        }
    }

    // ************** GUI **************
    // *******************************
    public static void GUI() {
        // Frame setup
        JFrame frame = new JFrame("Hospital Management System");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(1200, 700);
        frame.setLayout(new BorderLayout());

        // Header panel
        JPanel headerPanel = new JPanel();
        headerPanel.setBackground(Color.LIGHT_GRAY);
        headerPanel.setPreferredSize(new Dimension(600, 50));
        JLabel headerLabel = new JLabel("Hospital", SwingConstants.CENTER);
        headerLabel.setFont(new Font("Arial", Font.BOLD, 18));
        headerPanel.add(headerLabel);

        // Info panel
        JPanel infoPanel = new JPanel();
        infoPanel.setBackground(Color.LIGHT_GRAY);
        infoLabel = new JLabel("Informations");
        infoLabel.setFont(new Font("Arial", Font.PLAIN, 20));
        infoPanel.add(infoLabel);

        // Menu panel
        JPanel menuPanel = new JPanel();
        menuPanel.setBackground(Color.DARK_GRAY);
        menuPanel.setPreferredSize(new Dimension(150, 350));
        int optionCount = 6;
        menuPanel.setLayout(new GridLayout(optionCount, 1, 15, 10));

        // Menu button names and functions of the buttons
        String[] menuButtonNames = {"Create Rendezvous", "Show Doctors", "Show Sections", "Show Rendezvous", "Add Doctor", "Add Section"};
        for (int i = 1; i <= optionCount; i++) {
            JButton button = new JButton(menuButtonNames[i - 1]);
            button.setBackground(Color.WHITE);
            int finalI = i;
            button.addActionListener(_ -> {
                switch (finalI) {
                    case 1 -> makeRendezvous();
                    case 2 -> showDoctors();
                    case 3 -> showSections();
                    case 4 -> showRendezvous();
                    case 5 -> addDoctor();
                    case 6 -> addSection();
                }
            });
            menuPanel.add(button);
        }

        // Frame setup
        frame.add(headerPanel, BorderLayout.NORTH);
        frame.add(menuPanel, BorderLayout.WEST);
        frame.add(infoPanel, BorderLayout.CENTER);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }

    // MAKE RENDEZVOUS START
    public static void makeRendezvous() {
        try {
            // Get infos
            Section selectedSection = selectSection();
            if (selectedSection == null) return;

            Patient patient = getPatientInfo();
            if (patient == null) return;

            Doctor selectedDoctor = selectDoctor(selectedSection);
            if (selectedDoctor == null) return;

            Date date = selectDate();
            if (date == null) return;

            // Create rendezvous
            crs.makeRendezvous(patient.getNational_Id(), 1, selectedSection.getId(), selectedDoctor.getDiplomaID(), date);
            crs.saveTablesToDisk("tables.ser");

            JOptionPane.showMessageDialog(null, "Rendezvous created successfully!", "Success", JOptionPane.INFORMATION_MESSAGE);
            infoLabel.setText("Rendezvous created successfully for patient: " + patient.getName());
        } catch (IDException e) {
            JOptionPane.showMessageDialog(null, "The selected doctor is unavailable at the chosen time.", "Scheduling Error", JOptionPane.WARNING_MESSAGE);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, "Error: " + e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    private static Section selectSection() {
        List<Section> sections = crs.getHospital(1).getAllSections();
        String[] sectionNames = new String[sections.size()];
        for (int i = 0; i < sections.size(); i++) {
            sectionNames[i] = sections.get(i).getName();
        }

        String selectedSectionName = (String) JOptionPane.showInputDialog(
                null,
                "Select a Section:",
                "Section Selection",
                JOptionPane.QUESTION_MESSAGE,
                null,
                sectionNames,
                sectionNames[0]
        );

        if (selectedSectionName == null) return null;
        return crs.getHospital(1).getSectionByName(selectedSectionName);
    }

    private static Patient getPatientInfo() {
        String patientName = JOptionPane.showInputDialog(null, "Enter Patient Name:", "Patient Information", JOptionPane.QUESTION_MESSAGE);
        if (patientName == null || patientName.isEmpty()) {
            JOptionPane.showMessageDialog(null, "Patient name cannot be empty!", "Error", JOptionPane.ERROR_MESSAGE);
            return null;
        }
        // If patient name is not a string show error
        if (!patientName.matches("[a-zA-Z]+")) {
            JOptionPane.showMessageDialog(null, "Patient name must be string!", "Error", JOptionPane.ERROR_MESSAGE);
            return null;
        }

        String patientIdStr = JOptionPane.showInputDialog(null, "Enter Patient ID:", "Patient Information", JOptionPane.QUESTION_MESSAGE);
        if (patientIdStr == null || patientIdStr.isEmpty()) {
            JOptionPane.showMessageDialog(null, "Patient ID cannot be empty!", "Error", JOptionPane.ERROR_MESSAGE);
            return null;
        }

        // If patient ID is not 4 digits long or not a number show error
        if (patientIdStr.length() != 4 || !patientIdStr.matches("\\d+")) {
            JOptionPane.showMessageDialog(null, "Patient ID must be a 4-digit number!", "Error", JOptionPane.ERROR_MESSAGE);
            return null;
        }

        long patientId;
        try {
            patientId = Long.parseLong(patientIdStr);
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(null, "Invalid Patient ID format!", "Error", JOptionPane.ERROR_MESSAGE);
            return null;
        }

        try {
            Patient patient = new Patient(patientName, patientId);
            crs.addPatient(patient);
            return patient;
        } catch (DuplicateInfoException e) {
            JOptionPane.showMessageDialog(null, e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
            return null;
        }
    }

    private static Doctor selectDoctor(Section section) {
        if(section.getDoctors().isEmpty()){
            JOptionPane.showMessageDialog(null, "There are no doctors in this section!", "Error", JOptionPane.ERROR_MESSAGE);
            return null;
        }

        List<Doctor> doctors = section.getDoctors();
        String[] doctorNames = new String[doctors.size()];
        for (int i = 0; i < doctors.size(); i++) {
            doctorNames[i] = doctors.get(i).getName();
        }

        String selectedDoctorName = (String) JOptionPane.showInputDialog(
                null,
                "Select a Doctor:",
                "Doctor Selection",
                JOptionPane.QUESTION_MESSAGE,
                null,
                doctorNames,
                doctorNames[0]
        );

        if (selectedDoctorName == null) return null;
        return section.getDoctorByName(selectedDoctorName);
    }

    private static Date selectDate() {
        SpinnerDateModel dateModel = new SpinnerDateModel();
        JSpinner dateSpinner = new JSpinner(dateModel);
        dateSpinner.setEditor(new JSpinner.DateEditor(dateSpinner, "dd-MM HH"));

        int option = JOptionPane.showConfirmDialog(null, dateSpinner, "Select Date", JOptionPane.OK_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE);
        if (option == JOptionPane.OK_OPTION) {
            Date date = (Date) dateSpinner.getValue();
            // Java Date class starts from 1900 we need to add 1900 to the year
            // Old java rules
            date.setYear(2025 - 1900);
            date.setMinutes(0);
            return date;
        }
        return null;
    }
    // MAKE RENDEZVOUS FINISH

    public static void showDoctors() {
        // This code is copied from Internet String builder method*
        StringBuilder doctorsInfo = new StringBuilder("<html><body><h2>List of Doctors</h2><ul>");
        for (Doctor doctor : crs.getHospital(1).getAllDoctors()) {
            doctorsInfo.append("<li>")
                    .append("Doctor Name: ").append(doctor.getName())
                    .append(", ID: ").append(doctor.getDiplomaID())
                    .append(", Section: ").append(doctor.getSection().getName())
                    .append("</li>");
        }
        doctorsInfo.append("</ul></body></html>");
        infoLabel.setText(doctorsInfo.toString());
    }

    public static void showSections() {
        // This code is copied from Internet String builder method*
        StringBuilder sectionsInfo = new StringBuilder("<html><body><h2>List of Sections</h2><ul>");
        for (Section section : crs.getHospital(1).getAllSections()) {
            sectionsInfo.append("<li>")
                    .append("Section Name: ").append(section.getName())
                    .append(", ID: ").append(section.getId())
                    .append("</li>");
        }
        sectionsInfo.append("</ul></body></html>");
        infoLabel.setText(sectionsInfo.toString());
    }

    public static void showRendezvous() {
        // This code is copied from Internet String builder method*
        StringBuilder rendezvousInfo = new StringBuilder("<html><body><h2>List of Rendezvous</h2><table border='1' style='border-collapse: collapse; text-align: center;'>");
        rendezvousInfo.append("<tr><th>Date</th><th>Patient</th><th>Section</th><th>Doctor</th></tr>");
        for (Rendezvous rendezvous : crs.getHospital(1).getAllRendezvous()) {
            rendezvousInfo.append("<tr>")
                    .append("<td>").append(new SimpleDateFormat("dd-MM HH:mm").format(rendezvous.getDate())).append("</td>")
                    .append("<td>").append(rendezvous.getPatient().getName()).append("</td>")
                    .append("<td>").append(rendezvous.getSection().getName()).append("</td>")
                    .append("<td>").append(rendezvous.getDoctor().getName()).append("</td>")
                    .append("</tr>");
        }
        rendezvousInfo.append("</table></body></html>");
        infoLabel.setText(rendezvousInfo.toString());
    }

    public static void addDoctor() {
        if (crs.getHospital(1).getAllSections().isEmpty()) {
            JOptionPane.showMessageDialog(null, "There are no sections to add a doctor! First add section", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }
        String doctorName = JOptionPane.showInputDialog(null, "Enter Doctor Name:", "Doctor Information", JOptionPane.QUESTION_MESSAGE);
        if (doctorName == null || doctorName.isEmpty()) {
            JOptionPane.showMessageDialog(null, "Doctor name cannot be empty!", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }
        // If doctor name is not a string show error
        if (!doctorName.matches("[a-zA-Z ]+")) {
            JOptionPane.showMessageDialog(null, "Doctor name must be string!", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        String doctorIdStr = JOptionPane.showInputDialog(null, "Enter Doctor National ID:", "Doctor Information", JOptionPane.QUESTION_MESSAGE);
        if (doctorIdStr == null || doctorIdStr.isEmpty()) {
            JOptionPane.showMessageDialog(null, "Doctor ID cannot be empty!", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        // If doctor is not 4 digits long or not a number show error
        if (doctorIdStr.length() != 4 || !doctorIdStr.matches("\\d+")) {
            JOptionPane.showMessageDialog(null, "Doctor ID must be a 4-digit number!", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        String diplomaIDStr = JOptionPane.showInputDialog(null, "Enter Doctor Diploma ID:", "Doctor Information", JOptionPane.QUESTION_MESSAGE);
        if (diplomaIDStr == null || diplomaIDStr.isEmpty()) {
            JOptionPane.showMessageDialog(null, "Doctor Diploma ID cannot be empty!", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }
        int diplomaID;
        try {
            diplomaID = Integer.parseInt(diplomaIDStr);
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(null, "Invalid Doctor Diploma ID format!", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        int NID;
        try {
            NID = Integer.parseInt(doctorIdStr);
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(null, "Invalid Doctor ID format!", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        try {
            Section selectedSection = selectSection();
            if (selectedSection == null) return;

            Doctor doctor = new Doctor(doctorName, NID, diplomaID, selectedSection);
            selectedSection.addDoctor(doctor);
            crs.saveTablesToDisk("tables.ser");

            JOptionPane.showMessageDialog(null, "Doctor added successfully!", "Success", JOptionPane.INFORMATION_MESSAGE);
            infoLabel.setText("Doctor added successfully: " + doctor.getName());
        } catch (DuplicateInfoException e) {
            JOptionPane.showMessageDialog(null, "Doctor ID already exists!", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    public static void addSection() {
        String sectionName = JOptionPane.showInputDialog(null, "Enter Section Name:", "Section Information", JOptionPane.QUESTION_MESSAGE);
        if (sectionName == null || sectionName.isEmpty()) {
            JOptionPane.showMessageDialog(null, "Section name cannot be empty!", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }
        if (!sectionName.matches("[a-zA-Z ]+")) {
            JOptionPane.showMessageDialog(null, "Section name must only contain letters and spaces!", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        crs.getHospital(1).addSection(sectionName);
        crs.saveTablesToDisk("tables.ser");

        JOptionPane.showMessageDialog(null, "Section added successfully!", "Success", JOptionPane.INFORMATION_MESSAGE);
        infoLabel.setText("Section added successfully: " + sectionName);
    }





    // ************** CLI **************
    // *******************************
    public static void CLI() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("1. Make Rendezvous");
            System.out.println("2. Show Doctors");
            System.out.println("3. Show Sections");
            System.out.println("4. Show Rendezvous");
            System.out.println("5. Add Doctor");
            System.out.println("6. Add Section");
            System.out.println("7. Exit");

            int choice = scanner.nextInt();
            scanner.nextLine();
            switch (choice) {
                case 1 -> makeRendezvousCLI();
                case 2 -> showDoctorsCLI();
                case 3 -> showSectionsCLI();
                case 4 -> showRendezvousCLI();
                case 5 -> addDoctorCLI();
                case 6 -> addSectionCLI();
                case 7 -> System.exit(0);
                default -> System.out.println("Invalid choice. Please try again.");
            }
            // 2.3-second sleep
            try {
                Thread.sleep(2300);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }


    // MAKE RENDEZVOUS START
    public static void makeRendezvousCLI() throws RuntimeException{
        Scanner scanner = new Scanner(System.in);
        Section selectedSection;
        Patient patient;
        Doctor selectedDoctor;
        Date date;

        // Section info
        do {
            try {
                System.out.println("Select a Section: (1,2,3...)");
                int i = 1;
                for (Section section : crs.getHospital(1).getAllSections()) {
                    System.out.println(i + ".) " + section.getName());
                    i++;
                }
                int sectionChoice = scanner.nextInt();
                scanner.nextLine();
                if (sectionChoice < 1 || sectionChoice > crs.getHospital(1).getAllSections().size()) {
                    throw new RuntimeException("Invalid section choice");
                }
                selectedSection = crs.getHospital(1).getSection(sectionChoice);
                if (selectedSection.getDoctors().isEmpty()) {
                    System.out.println("There are no doctors in this section. Please select another section.");
                    continue;
                }
                break;
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        }while(true);

        // Patient info
        do {
            try {
                System.out.println("Enter Patient Name:");
                String patientName = scanner.nextLine();
                if (patientName.isEmpty()) {
                    throw new RuntimeException("Patient name cannot be empty");
                }
                System.out.println("Enter Patient ID:");
                long patientID = scanner.nextLong();
                scanner.nextLine();
                if (patientID < 1000 || patientID > 9999) {
                    throw new RuntimeException("Patient ID must be a 4-digit number");
                }
                if (crs.getPatients().containsKey(patientID)) {
                    throw new DuplicateInfoException("This patient already exists");
                }

                patient = new Patient(patientName, patientID);
                crs.addPatient(patient);
                break;

            } catch (DuplicateInfoException e) {
                System.out.println(e.getMessage());
            } catch (Exception e) {
                System.out.println("Error: " + e.getMessage());
            }
        } while (true);

        // Doctor info
        do {
            try {
                System.out.println("Select a Doctor: (1,2,3...)");
                // Print all doctors in the selected section
                int i = 1;
                List<Doctor> doctors = selectedSection.getDoctors();
                for (Doctor doctor : doctors) {
                    System.out.println(i + ".) " + doctor.getName());
                    i++;
                }
                int doctorChoice = scanner.nextInt();
                scanner.nextLine();

                if (doctorChoice < 1 || doctorChoice > doctors.size()) {
                    throw new RuntimeException("Invalid doctor choice");
                }

                selectedDoctor = doctors.get(doctorChoice - 1);
                break;
            } catch (Exception e) {
                throw new RuntimeException("Error during doctor selection: " + e.getMessage());
            }
        }while(true);

        // Date info
        do{
            try {
                System.out.println("Enter Date (dd-MM HH):");
                String dateString = scanner.nextLine();
                date = new SimpleDateFormat("dd-MM HH").parse(dateString);
            } catch (Exception e) {
                throw new RuntimeException("Invalid date format: " + e.getMessage());
            }

            try {
                crs.makeRendezvous(patient.getNational_Id(), 1, selectedSection.getId(), selectedDoctor.getDiplomaID(), date);
                crs.saveTablesToDisk("tables.ser");
                System.out.println("Rendezvous created successfully for patient: " + patient.getName());
                break;
            }catch (FullScheduleException e) {
                System.out.println(e.getMessage());
            } catch (Exception e) {
                throw new RuntimeException("Error during rendezvous creation: " + e.getMessage());
            } catch (IDException e) {
                throw new RuntimeException(e);
            }
        }while(true);
    }
    // MAKE RENDEZVOUS FINISH


    public static void showDoctorsCLI() {
        System.out.println("List of Doctors:");
        for (Doctor doctor : crs.getHospital(1).getAllDoctors()) {
            System.out.println(doctor.toString());
        }
    }

    public static void showSectionsCLI() {
        System.out.println("List of Sections:");
        for (Section section : crs.getHospital(1).getAllSections()) {
            System.out.println(section.toString());
        }
    }

    public static void showRendezvousCLI() {
        System.out.println("List of Rendezvous:");
        for (Rendezvous rendezvous : crs.getHospital(1).getAllRendezvous()) {
            System.out.println(rendezvous.toString());
        }
    }

    public static void addDoctorCLI() {
        if (crs.getHospital(1).getAllSections().isEmpty()) {
            System.out.println("There are no sections to add a doctor! First add section");
            return;
        }
        do{
            try {
                Scanner scanner = new Scanner(System.in);
                System.out.println("Enter Doctor Name:");
                String doctorName = scanner.next();
                if(doctorName.isEmpty()){
                    throw new RuntimeException("Doctor name cannot be empty");
                }
                System.out.println("Enter Doctor National ID:");
                int doctorID = scanner.nextInt();
                scanner.nextLine();

                System.out.println("Enter Doctor Diploma ID:");
                int diplomaID = scanner.nextInt();
                scanner.nextLine();

                // Show sections and get the selected section
                Section selectedSection;
                System.out.println("Select a Section: (1,2,3...)");
                int i = 1;
                for (Section section : crs.getHospital(1).getAllSections()) {
                    System.out.println(i+ ".)" + section.getName());
                    i++;
                }
                int sectionChoice = scanner.nextInt();
                scanner.nextLine();
                if(sectionChoice < 1 || sectionChoice > crs.getHospital(1).getAllSections().size()){
                    throw new RuntimeException("Invalid section choice");
                }
                selectedSection = crs.getHospital(1).getSection(sectionChoice);


                Doctor doctor = new Doctor(doctorName, doctorID, diplomaID, selectedSection);
                selectedSection.addDoctor(doctor);
                crs.saveTablesToDisk("tables.ser");
                break;
            } catch (Exception e) {
                System.out.println("Error: " + e.getMessage());
            } catch (DuplicateInfoException e) {
                System.out.println(e.getMessage());
            }
        }while(true);
    }

    public static void addSectionCLI() {
        try {
            Scanner scanner = new Scanner(System.in);
            System.out.println("Enter Section Name:");
            String sectionName = scanner.nextLine();
            if(sectionName.isEmpty()){
                throw new RuntimeException("Section name cannot be empty");
            }
            crs.getHospital(1).addSection(sectionName);

            crs.saveTablesToDisk("tables.ser");
            System.out.println("Section added successfully: " + sectionName);
        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

}