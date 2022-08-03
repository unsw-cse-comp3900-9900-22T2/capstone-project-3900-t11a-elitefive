import { Box, Typography, Modal } from '@mui/material'
import React, { useEffect, useState } from 'react'
import PopupContainer from '../../components/PopupContainer'
import ReusableButton from '../../components/ReusableButton'
import { useGameState } from '../../global/GlobalGameState'
import { useNavigate } from 'react-router-dom'


type Props = {}

export default function PopupStatefulComponent({}: Props) {
  const navigate = useNavigate();
  const { gameOverState, setWinner } = useGameState();
  const [popup, setPopup] = useState(true);

  const handleReturn = () => {
    navigate('/dashboard');
  }

  // {(gameOverState.winner) && (


  // HACKY way
  // !!gameOverState.winner checks is gameOverState.winner != Undefined
  // True && True = True when there is a winner
  // False && True when there is not a winner

  // when the user clicks out of the modal popup: popup -> false
  // therefore True && false == false, therefore modal closes
  return (
    <Modal open={!!gameOverState.winner && popup} onClose={() => {setPopup(false)}}>
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
    </Modal>
  )
}