import React from 'react'
import styled from 'styled-components';
import { Typography } from '@mui/material';
import { useNavigate } from 'react-router-dom';

import YavalathSvg from '../assets/YavalathSVG.svg';

type Props = {}

const Container = styled.div`
  padding: 10px;
  display: flex;
  align-items: center;
  & > img {
    width: 90px;
    height: 60px;
  }
`;

const ContainerYavalath = styled.div`
  position: absolute;
  top: 5%;
  left: 10%;
  display: flex;
  flex-direction: column;
  cursor: pointer;
`;

export default function YavalathButton({}: Props) {
  const navigate = useNavigate();
  return (
    <Container onClick={() => navigate("/dashboard")}>
      <img src={YavalathSvg}/>
      <Typography variant="h4">Yavalath</Typography>
    </Container>
  )
}

export function YavalathButtonFixed({}: Props) {
  const navigate = useNavigate();
  return (
    <ContainerYavalath>
      <Container onClick={() => navigate("/dashboard")}>
        <img src={YavalathSvg}/>
        <Typography variant="h4">Yavalath</Typography>
      </Container>
    </ContainerYavalath>
  )
}