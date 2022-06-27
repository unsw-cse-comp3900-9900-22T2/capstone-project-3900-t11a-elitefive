import { Box, Typography } from '@mui/material'
import React from 'react'
import PopupContainer from '../../components/PopupContainer'
import ReusableButton from '../../components/ReusableButton'
import { useGameState } from '../../global/GlobalGameState'
import { useNavigate } from 'react-router-dom'


type Props = {}

export default function PopupStatefulComponent({}: Props) {
  const navigate = useNavigate();
  const { gameOverState, setWinner } = useGameState();

  const handleReturn = () => {
    navigate('/dashboard');
  }
  return (
    <>
    {(gameOverState.winner) && (
      <PopupContainer>
        <Box
          width="100%" 
          height="100%" 
          display="flex" 
          flexDirection="column" 
          justifyContent="center" 
          alignItems="center"
          gap="30px"
        >
          <Typography variant="h3">Winner: {gameOverState.winner}</Typography>
          <ReusableButton 
            onClick={handleReturn}
            background="var(--accent-dark)"
          >
            to dashboard
          </ReusableButton>
        </Box>
      </PopupContainer>
    )}
    </>
  )
}