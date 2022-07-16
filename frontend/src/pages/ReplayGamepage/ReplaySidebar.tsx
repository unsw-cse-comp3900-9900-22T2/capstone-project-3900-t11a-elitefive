import React from 'react'
import styled from 'styled-components';
import BasicCard, {Card2, Card3, ReplayPlayerCard} from '../../components/ReusableCard';
import Typography from '@mui/material/Typography';
import Button, {Button2} from '../../components/ReusableButton';
import { useGameState } from '../../global/GlobalGameState';
import StickyHeadTable from '../../components/Tables';

type Props = {}

const Container = styled.div`
  display: flex;
  flex-direction: column;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

const ContainerArrow = styled.div`
  display: flex;
  flex-direction: row;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;


export default function Sidebar({}: Props) {
  const { setWinner } = useGameState();
  return (
    <Container>
      <Typography sx={{ fontSize: 80 }} color="white" gutterBottom>
          Replay
        </Typography>
      <ReplayPlayerCard>
        <Typography sx={{ fontSize: 30 }} color="white" gutterBottom>
          Jacky
        </Typography>
        <Typography sx={{ fontSize: 30 }} color="white" gutterBottom>
          David
        </Typography>
      </ReplayPlayerCard>
      <Card2>
        <StickyHeadTable/>
      </Card2>
      <ContainerArrow>
        <Button>Back</Button>
        <Button>Next</Button>
      </ContainerArrow>
      <Button 
        width={350} 
        height={40}
        background="red"
        onClick={() => { setWinner("COMPUTER") }}
      >
        Retire
      </Button>
    </Container>
  )
}