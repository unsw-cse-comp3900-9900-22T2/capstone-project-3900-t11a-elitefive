import React from 'react'
import styled from 'styled-components';
import BasicCard, {Card2, Card3} from '../../components/ReusableCard';
import Typography from '@mui/material/Typography';
import Button, {Button2} from '../../components/ReusableButton';
import { PlayerType, useGameState } from '../../global/GlobalGameState';
import { useSocket } from '../../global/GlobalWS';
import { useAuth } from '../../global/GlobalAuth';

type Props = {}

const Container = styled.div`
  display: flex;
  flex-direction: column;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

export default function Sidebar({}: Props) {
  const { setWinner, getPlayers } = useGameState();
  const {  emit } = useSocket();
  const { getUID } = useAuth();

  const currPlayer = getPlayers()[0]?.name;
  const currPlayerElo = getPlayers()[0]?.elo;
  const otherPlayer = getPlayers()[1]?.name;
  const otherPlayerElo = getPlayers()[1]?.elo;

  return (
    <Container>
      <Card3>
        <Typography sx={{ fontSize: 40 }} color="white" gutterBottom>
          Player { currPlayer }
        </Typography>
        <Typography sx={{ fontSize: 20 }} color="white" gutterBottom>
          {currPlayerElo}
        </Typography>
      </Card3>
      <Card2>
        <Typography sx={{ fontSize: 40 }} color="white" gutterBottom>
          Your Turn 
        </Typography>
      </Card2>
      <Card3>
        <Typography sx={{ fontSize: 40 }} color="white" gutterBottom>
          Player { otherPlayer }
        </Typography>
        <Typography sx={{ fontSize: 20 }} color="white" gutterBottom>
          {otherPlayerElo}
        </Typography>
      </Card3>
      <Button 
        width={350} 
        height={40}
        background="red"
        onClick={() => { 
          emit("retire", JSON.stringify({
            'uid': getUID()
          }))
        }}
      >
        Retire
      </Button>
    </Container>
  )
}