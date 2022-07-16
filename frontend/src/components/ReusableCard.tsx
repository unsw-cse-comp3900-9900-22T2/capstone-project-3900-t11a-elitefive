import React from 'react'
import styled from 'styled-components';

// For basic card
import Box from '@mui/material/Box';
import Card from '@mui/material/Card';
import CardActions from '@mui/material/CardActions';

// For Card 2/3
import CardContent from '@mui/material/CardContent';
import Button from '@mui/material/Button';
import Typography from '@mui/material/Typography';
import {StyledCard, cardProps} from './ReusableCard-styled';
import HexCell from './HexCell';

import {Routes, Route, useNavigate} from 'react-router-dom';

type Props = {
  children?: React.ReactElement | any;
} & cardProps;

const Container = styled.div`
  display:flex;
  flex-direction: column;
  justify-content: left;
  align-items: right;
  grid-gap: 15px;
`;

const Container2 = styled.div`
  display:flex;
  flex-direction: row;
  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

const StatContainer = styled.div`
  display: flex;
  background: var(--accent-dark);
  height: 300px;
  width: 300px;
  border-radius: 10px;
`;

const bull = (
    <Box
      component="span"
      sx={{ display: 'inline-block', mx: '2px', transform: 'scale(0.8)' }}
    >
      •
    </Box>
  );

  // Basic Card (Delete just for reference)
  export default function Basicard() {
    return (
      <Card sx={{ minWidth: 275 }}>
        <CardContent>
          <Typography sx={{ fontSize: 14 }} color="text.secondary" gutterBottom>
            Word of the Day
          </Typography>
          <Typography variant="h5" component="div">
            be{bull}nev{bull}o{bull}lent
          </Typography>
          <Typography sx={{ mb: 1.5 }} color="text.secondary">
            adjective
          </Typography>
          <Typography variant="body2">
            well meaning and kindly.
            <br />
            {'"a benevolent smile"'}
          </Typography>
        </CardContent>
        <CardActions>
          <Button size="small">Learn More</Button>
        </CardActions>
      </Card>
    );
  }

  // For Your Turn card
  export function Card2({children, ...styleProps}: Props) {
    return (
      <StyledCard
        {...styleProps}
      >
        <CardContent>
          {children}
        </CardContent>
      </StyledCard>
    );
  }

  // For player card
  export function Card3({children, ...styleProps}: Props) {
    return (
      <StyledCard
        {...styleProps}
      >
        <CardContent>
          <Container2>
            <HexCell
            />
            <Container>
              {children}
            </Container>
          </Container2>
        </CardContent>
      </StyledCard>
    );
  }

  export function statCard({children, ...styleProps}: Props) {
    return (
      <StyledCard
        {...styleProps}
      >
        <CardContent>
          <StatContainer>
            <HexCell
            />
            <Container>
              {children}
            </Container>
          </StatContainer>
        </CardContent>
      </StyledCard>
    )
  }

  export function MatchCard() {

  const navigate = useNavigate();

  const navigateToReplayGamepage = () => {
      // 👇️ navigate to /contacts
      navigate('/replaygame');
  };

    return (
      <Card sx={{ minWidth: 800 }}>
        <Container2>
          <CardContent>
            <Typography variant="h5" component="div">
              Jacky
            </Typography>
            <Typography variant="h5" component="div">
              Yirong
            </Typography>
            <Typography variant="h5" component="div">
              David
            </Typography>
          </CardContent>
          <CardActions>
            <Button onClick={navigateToReplayGamepage}>Watch Replay</Button>
          </CardActions>
        </Container2>
      </Card>
    );
  }

  // For player card
  export function ReplayPlayerCard({children, ...styleProps}: Props) {
    return (
      <StyledCard
        {...styleProps}
      >
        <CardContent>
          <Container2>
            <Container>
              {children}
            </Container>
          </Container2>
        </CardContent>
      </StyledCard>
    );
  }

