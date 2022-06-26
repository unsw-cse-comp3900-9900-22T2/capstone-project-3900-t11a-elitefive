import React from 'react'
import styled from 'styled-components';
import {Routes, Route, useNavigate} from 'react-router-dom';

// local
import Board from '../components/Board';
import YavalathButton from '../components/YavalathButton';
import BasicCard, {Card2, Card3} from '../components/ReusableCard';
import Typography from '@mui/material/Typography';
import Button, {Button2} from '../components/ReusableButton';

type Props = {}

const Container = styled.div`

`;

const Container2 = styled.div`
  display: flex;
  flex-direction: row;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

const Container3 = styled.div`
  display: flex;
  flex-direction: column;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

export default function Gamepage({}: Props) {
  const navigate = useNavigate();
  const navigateToDashboard = () => {
    // 👇️ navigate to /contacts
    navigate('/dashboard');
  };
  
  return (
    <Container>
      <YavalathButton/>
      <Container2>
        <Board />
        <Container3>
          <Card3>
            <Typography sx={{ fontSize: 40 }} color="white" gutterBottom>
              Player 1
            </Typography>
            <Typography sx={{ fontSize: 20 }} color="white" gutterBottom>
              1580
            </Typography>
          </Card3>
          <Card2>
            <Typography sx={{ fontSize: 40 }} color="white" gutterBottom>
              Your Turn
            </Typography>
          </Card2>
          <Card3>
            <Typography sx={{ fontSize: 40 }} color="white" gutterBottom>
              Player 2
            </Typography>
            <Typography sx={{ fontSize: 20 }} color="white" gutterBottom>
              1530
            </Typography>
          </Card3>
          <Button 
            width={350} 
            height={40}
            background="red"
            onClick={navigateToDashboard}
          >
            Retire
          </Button>
        </Container3>
      </Container2>
    </Container>
  )
}