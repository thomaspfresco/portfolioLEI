package com.example.demo;    

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import com.example.data.User;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service    
public class UserService   
{    
    @Autowired    
    private UserRepository userRepository;

    public List<User> getAllUsers()  
    {    
        List<User>userRecords = new ArrayList<>();    
        userRepository.findAll().forEach(userRecords::add);    
        return userRecords;    
    }

    public void addUser(User user) {    
        List<User> p = findByNameEndsWith(user.getUsername());
        if(!p.contains(user)) {
            p.add(user);
        }
        userRepository.save(user);    
    }

    public Optional<User> getUser(int id) {
        return userRepository.findById(id);
    }


    public List<User> findByNameEndsWith(String chars) {
        return userRepository.findByNameEndsWith(chars);
    }

    public List<User> findByUsername(String username) {
        return userRepository.findByUsername(username);
    }

    public List<User> findByLogged(Boolean logged) {
        return userRepository.findByLogged(logged);
    }

    void updateLogin(int id, boolean l) {
        userRepository.updateLogin(id,l);
    }
}    