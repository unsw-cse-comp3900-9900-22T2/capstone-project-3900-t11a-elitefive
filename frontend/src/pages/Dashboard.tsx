import React from 'react';
import styled from 'styled-components';
import Board from '../components/Board';
import { Typography } from '@mui/material';
import StyledInput from '../components/StyledInput';
import {StyledButton} from '../components/ReusableButton-styled';
import Button, {Button2, LargeButton} from '../components/ReusableButton';
import {Routes, Route, useNavigate} from 'react-router-dom';
import YavalathButton from '../components/YavalathButton';

type Props = {}

const Container  = styled.div`
  min-height: 100vh;

  display: flex;
  flex-direction: Column;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`

const Container1  = styled.div`
  min-height: 100vh;

  display: flex;
  flex-direction: row;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`

export default function Dashboard({}: Props) {
  return (
    <Container>
      <Container1>
        <LargeButton>
          <Typography variant="h3">{"Play AI"}</Typography>
        </LargeButton>
        <LargeButton>
          <Typography variant="h3">{"Leaderboard"}</Typography>
        </LargeButton>
        <LargeButton>
          <Typography variant="h3">{"Replays"}</Typography>
        </LargeButton>
      </Container1>
    </Container>
  )
}