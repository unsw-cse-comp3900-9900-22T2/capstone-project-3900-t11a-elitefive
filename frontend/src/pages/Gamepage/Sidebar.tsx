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

const Highlight = styled.div<{isHighlighted: boolean}>`
  ${props => props.isHighlighted && `border: 5px solid var(--accent-blue)`};
`;

export default function Sidebar({}: Props) {
  const { setWinner, getPlayers, getCurrentPlayer, getNumberPlayers } = useGameState();
  const {  emit } = useSocket();
  const { getUID } = useAuth();

  return (
    <Container>
      {getPlayers().slice(0,getNumberPlayers()).map((player, index) => {
        const currPlayer = player.name;
        const currPlayerElo = player.elo;
        return (
          <Highlight isHighlighted={getCurrentPlayer() === index}>
          <Card3>
            {console.log(getCurrentPlayer())}
            <Typography sx={{ fontSize: 40 }} color="white" gutterBottom>
              Player { currPlayer }
            </Typography>
            <Typography sx={{ fontSize: 20 }} color="white" gutterBottom>
              {currPlayerElo}
            </Typography>
          </Card3>
          </Highlight>
        )
      })}
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