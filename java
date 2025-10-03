import javax.crypto.Cipher; 
import javax.crypto.spec.SecretKeySpec; 
import java.util.Scanner; 
 
public class aes { 
    public static void main(String[] args) {         
        Scanner scanner = new Scanner(System.in); 
        System.out.print("Enter plain text: "); 
        String plainText = scanner.nextLine(); 
        System.out.print("Enter key (must be 16 bytes long): "); 
        String keyString = scanner.nextLine();         
        String key = padKey(keyString);         
        try { 
            byte[] encrypted = encrypt(plainText, key); 
            System.out.println("Cipher text: " + bytesToHex(encrypted)); 
            String decrypted = decrypt(encrypted, key); 
            System.out.println("Decrypted text: " + decrypted);             
            scanner.close();         
        } 
        catch (Exception e) { 
            e.printStackTrace(); 
        } 
    } 
 
    public static byte[] encrypt(String plainText, String key) throws Exception { 
        SecretKeySpec secretKey = new SecretKeySpec(key.getBytes(), "AES");         
        Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");         
        cipher.init(Cipher.ENCRYPT_MODE, secretKey);         
        return cipher.doFinal(plainText.getBytes()); 
    } 
 
    public static String decrypt(byte[] cipherText, String key) throws Exception { 
        SecretKeySpec secretKey = new SecretKeySpec(key.getBytes(), "AES");         
        Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");         
        cipher.init(Cipher.DECRYPT_MODE, secretKey);         
        byte[] decryptedBytes = cipher.doFinal(cipherText);         
        return new String(decryptedBytes); 
    } 
 
    public static String bytesToHex(byte[] bytes) {         
        StringBuilder result = new StringBuilder();         
        for (byte aByte : bytes) { 
            result.append(Integer.toString((aByte & 0xff) + 0x100, 16).substring(1)); 
        } 
        return result.toString(); 
    } 
 
    public static String padKey(String key) { 
        StringBuilder paddedKey = new StringBuilder(key);         
        while (paddedKey.length() < 16) {             
            paddedKey.append('\0'); 
        } 
        return paddedKey.toString(); 
    } 
}