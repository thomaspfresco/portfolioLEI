package com.example.demo;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;

import java.util.List;

import com.example.data.Team;

public interface TeamRepository extends CrudRepository<Team, Integer>   
{ 
    @Query("select t from Team t where t.name like %?1")
    public List<Team> getTeamByName(String chars);
} 