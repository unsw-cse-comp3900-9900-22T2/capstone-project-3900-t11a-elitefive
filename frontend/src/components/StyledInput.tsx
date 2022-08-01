import React from 'react'
import styled from 'styled-components';
import { TextField, Typography, TextFieldProps } from '@mui/material'; 

type Props = {
  label?: string;
  password?: boolean;
  value?: string;
  onChange?: (e: React.ChangeEvent<HTMLInputElement>) => void;
} & styledProps;

type styledProps = {
  background?: string;
  size?: "small" | "medium";
  width?: number;
  disabled?: boolean;
}

const Container = styled.div`
  display:flex;
  flex-direction: column;
`;

const StyledTextField = styled.div<styledProps>`
  background: ${({background}) => background ? background : "var(--textbox-dark)"};

`;


export default function({ label, password, value, onChange, ...styledProps }: Props) {

  // TODO make border white
  const inlineStyleOverride = {
    style: {
      color: 'white',
    }
  }

  const { disabled } = styledProps;

  return (
    <Container>
      <Typography variant="subtitle1">{label}</Typography>
      <StyledTextField
        {...styledProps}
      >
        <TextField
          value={value}
          onChange={onChange}
          type={password ? "password": "required"}
          inputProps={{...inlineStyleOverride}}
          size={styledProps.size ? styledProps.size : undefined}
          style={{
            width: styledProps.width ? styledProps.width : 350
          }}
          disabled={disabled}
          placeholder="Select Player filter to enable"
        />
      </StyledTextField>
    </Container>
  )
}