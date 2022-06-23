package com.example.demo;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

import com.example.data.User;

public interface UserRepository extends CrudRepository<User, Integer> {
    @Query("select p from Player p where p.name like %?1")
    public List<User> findByNameEndsWith(String chars);
    public List<User> findByUsername(String username);
    public List<User> findByLogged(Boolean logged);

    @Transactional
    @Modifying
    @Query("update User user set user.logged=:l where user.id=:id")
    void updateLogin(int id, Boolean l);
}    