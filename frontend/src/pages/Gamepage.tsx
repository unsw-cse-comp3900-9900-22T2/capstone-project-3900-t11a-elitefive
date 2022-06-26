import React from 'react'
import styled from 'styled-components';
import {Routes, Route, useNavigate} from 'react-router-dom';

// local
import Board from '../components/Board';
import YavalathButton from '../components/YavalathButton';

type Props = {}

const Container = styled.div`

`;

export default function Gamepage({}: Props) {
  const navigate = useNavigate();
  const navigateToGame = () => {
    // 👇️ navigate to /contacts
    navigate('/game');
  };
  
  return (
    <Container>
      <YavalathButton/>
      <Board />
    </Container>
  )
}