package com.example.data;

//import javax.persistence.CascadeType;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.OneToOne;
//import javax.persistence.ManyToOne;
//import javax.xml.bind.annotation.XmlElement;
//import javax.xml.bind.annotation.XmlElementWrapper;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlElementWrapper;

@Entity
public class Match {
    @Id @GeneratedValue(strategy = GenerationType.AUTO)
    private int id;
    
    @OneToOne
    private Team teamA;
    
    @OneToOne
    private Team teamB;
    
    private String location;
    private String date;
    private Boolean finished = false;
    
    public Match() {
    }
    
    public Match(Team teamA, Team teamB, String location, String date, Boolean finished) {
        this.teamA = teamA;
        this.teamB = teamB;
        this.location = location;
        this.date = date;
        this.finished = finished;
    }
    
    public int getId() {
        return this.id;
    }
    
    public void setId(int id) {
        this.id = id;
    }
    
    @XmlElementWrapper(name = "teams")
    @XmlElement(name = "teamA")
    public Team getTeamA() {
        return teamA;
    }
    
    @XmlElementWrapper(name = "teams")
    @XmlElement(name = "teamB")
    public Team getTeamB() {
        return teamB;
    }

    public void setTeamA (Team teamA) {
        this.teamA = teamA;
    }

    public void setTeamB (Team teamB) {
        this.teamB = teamB;
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public Boolean getFinished() {
        return finished;
    }

    public void setFinished(Boolean finished) {
        this.finished = finished;
    }

    public String toString() {
        return this.teamA + " vs " + this.teamB + " at " + this.location + " at the date " + this.date;
    }
}
