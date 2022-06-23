package com.example.data;

//import javax.persistence.CascadeType;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
//import javax.persistence.OneToOne;
//import javax.persistence.ManyToOne;
//import javax.xml.bind.annotation.XmlElement;
//import javax.xml.bind.annotation.XmlElementWrapper;
import javax.persistence.Table;

@Entity
@Table(name="users")
public class User {
    @Id @GeneratedValue(strategy = GenerationType.AUTO)
    private int id;

    private String username; //inicio e fim de jogo, golo, cartoes, ...
    private String password; //0-90 + compensacao
    private String email;
    private int telefone;
    private Boolean admin;
    private Boolean logged = false;
    
    public User() {
    }
    
    public User(String username, String password, String email, int telefone, Boolean admin) {
        this.username = username;
        this.password = password;
        this.email = email;
        this.telefone = telefone;
        this.admin = admin;
        this.logged = false;
    }

    public User(String username, String password, String email, int telefone) {
        this.username = username;
        this.password = password;
        this.email = email;
        this.telefone = telefone;
        this.admin = false;
        this.logged = false;
    }

    public int getId() {
        return this.id;
    }
    
    public void setId(int id) {
        this.id = id;
    }
    
    public String getUsername() {
        return username;
    }
    
    public String getPassword() {
        return password;
    }

    public String getEmail() {
        return email;
    }

    public int getTelefone() {
        return telefone;
    }

    public Boolean getAdmin() {
        return admin;
    }

    public Boolean getLogged() {
        return logged;
    }

    public void setUsername (String username) {
        this.username = username;
    }

    public void setPassword (String password) {
        this.password = password;
    }

    public void setEmail (String email) {
        this.email = email;
    }

    public void setTelefone (int telefone) {
        this.telefone = telefone;
    }

    public void setAdmin (Boolean admin) {
        this.admin = admin;
    }

    public void setLogged (Boolean logged) {
        this.logged = logged;
    }

    public String toString() {
       return this.username;
    }
}
