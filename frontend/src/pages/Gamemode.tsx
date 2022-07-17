import React from 'react';
import styled from 'styled-components';
import Board from '../components/Board';
import { Typography } from '@mui/material';
import StyledInput from '../components/StyledInput';
import {StyledButton} from '../components/ReusableButton-styled';
import Button, {Button2, LargeButton} from '../components/ReusableButton';
import {Routes, Route, useNavigate} from 'react-router-dom';
import YavalathButton, { YavalathButtonFixed } from '../components/YavalathButton';
import ProfileWidget from '../components/ProfileWidget';
import ScrollWidget from '../components/ScrollWidget';
import ButtonGroup from '@mui/material/ButtonGroup';
import ReusableToggleButtonGroup from '../components/ReusableToggleButtonGroup';
import ReusableToggleButton from '../components/ReusableToggleButton';
import ToggleButton from '@mui/material/ToggleButton';

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
  min-height: 50vh;

  display: flex;
  flex-direction: row;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`

export default function Gamemode({}: Props) {

    const navigate = useNavigate();

    const navigateToGamepage = () => {
        // 👇️ navigate to /contacts
        navigate('/game');
    };

    return (
        <Container>
          <YavalathButtonFixed/>
          <ProfileWidget/>
          <Typography variant="h3">{"Select Game Mode"}</Typography>
          <ReusableToggleButtonGroup>
            <ToggleButton value="Vs Ai">Vs Ai</ToggleButton>
            <ToggleButton value="Vs Human">Vs Human</ToggleButton>
          </ReusableToggleButtonGroup>
          <ReusableToggleButtonGroup>
            <ToggleButton value="Ranked">Ranked</ToggleButton>
            <ToggleButton value="Casual">Casual</ToggleButton>
          </ReusableToggleButtonGroup>
          <Container1>
            <LargeButton onClick={navigateToGamepage}>
              <Typography variant="h3">{"Classic"}</Typography>
            </LargeButton>
            <LargeButton>
              <Typography variant="h3">{"Triplets"}</Typography>
            </LargeButton>
            <LargeButton>
              <Typography variant="h3">{"Pothole"}</Typography>
            </LargeButton>
          </Container1>
        </Container>
      )
}